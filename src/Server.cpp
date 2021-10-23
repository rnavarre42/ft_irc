#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <exception>
#include <csignal>
#include <cstdlib>
#include "Server.hpp"
#include "User.hpp"

Server	*Server::instance = NULL;

void	Server::signalHandler(int sig)
{
	Server &server = Server::getInstance();

	std::cerr << "server = " << std::hex << &server << std::endl;
	std::cerr << "server has " << server.count() << std::endl;
	if (sig == SIGINT)
	{
//		log(LOG_CONNECT, "Disconnecting clients...");
	}
	Server::instance->quit("Shutdown. Please, reconnect to another server.");
}

const char	*Server::ServerFullException::what(void) const throw()
{
	return ("The server is full.");
}

Server::Server(std::string ip, int port)
	: ip(ip), port(port)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, Server::signalHandler);
	std::memset(this->pollfds, '\0', sizeof(struct pollfd) * (MAXUSERS + 1));
	this->stop = false;
	this->initSocket();
	this->timeout = 1000;
	this->_bind();
}

Server::~Server(void)
{}

Server	&Server::getInstance(void)
{
	std::cerr << "getInstance = " << std::hex << Server::instance << std::endl;
	return (*Server::instance);
}
Server	&Server::getInstance(std::string ip, int port)
{
	if (Server::instance == NULL)
		Server::instance = new Server(ip, port);
	std::cerr << "getInstance = " << std::hex << Server::instance << std::endl;
	return (*Server::instance);
}

int		Server::count(void)
{
	return (this->fdMap.size());
}

void	Server::sendTo(std::string msg)
{
	int left;

	left = this->fdMap.size();
	for (size_t i = 1; left; i++)
	{
		if (this->pollfds[i].fd)
		{
			this->fdMap[this->pollfds[i].fd]->sendTo(msg);
			left--;
		}
	}
	
//	for (std::vector<User *>::iterator it = userVector.begin(); it != userVector.end(); it++)
//		it->sendTo(msg);
}

void	Server::quit(std::string msg)
{
//	for (std::vector<User *>::iterator it = userVector.begin(); it != userVector.end(); it++)
//	{
//		it->sendTo(msg);
//		it->disconnect();
//	}
	int left;
	std::cerr << "que nos vamos con " << this->fdMap.size() << " clientes dentro" << std::endl;

	left = this->fdMap.size();
	for (size_t i = 1; left; i++)
	{
		if (this->pollfds[i].fd)
		{
			this->fdMap[this->pollfds[i].fd]->sendTo(msg);
			this->_delUser(*this->fdMap[this->pollfds[i].fd]);
			left--;
		}
	}
	this->stop = true;
}

void	Server::start(void)
{
	this->_listen();
	this->loop();
}

void	Server::initSocket(void)
{
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (!this->fd)
	{
		std::cerr << "Server::initSocket function socket() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	this->opt = 1;
	this->addrlen = sizeof(this->address);
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &this->opt, sizeof(this->opt)))
	{
		std::cerr << "Server::initSocket function setsockopt() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int		Server::findFreePollIndex(void)
{
	for (int i = 1; i < MAXUSERS + 1; i++)
	{
		if (this->pollfds[i].fd == 0)
			return (i);
	}
	return (0);
}

User	&Server::_accept(void)
{
	User	*user;
	int		newFd;

	newFd = accept(this->fd, (struct sockaddr *)&this->address, (socklen_t *)&this->addrlen);
	if (newFd < 0)
	{
		std::cerr << "Server::accept function accept() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (this->fdMap.size() == MAXUSERS)
	{
		send(newFd, "The server is full. Please, try again more later.\r\n", 52, 0);
		close(newFd);
		throw Server::ServerFullException();
	}
	user = new User(newFd, *this);
	user->setPollIndex(findFreePollIndex());
	std::cerr << "setPollIndex = " << user->getPollIndex() << std::endl;
	this->pollfds[user->getPollIndex()].fd = newFd;
	this->pollfds[user->getPollIndex()].events = POLLIN;
	return (*user);
}

void	Server::_bind(void)
{
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->port);
	if (bind(this->fd, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
	{
		std::cerr << "Server::bind function bind() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::_listen(void)
{
	if (listen(this->fd, MAXLISTEN) < 0)
	{
		std::cerr << "Server::listen function listen() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	this->pollfds[0].fd = this->fd;
	this->pollfds[0].events = POLLIN;
}

int		Server::_poll(void)
{
	return (poll(this->pollfds, MAXUSERS + 1, this->timeout));
}

void	Server::_addUser(User &user)
{
//	this->userVector.push_back(&user);
	this->fdMap[user.getFd()] = &user;
	std::cerr << "Server::_addUser() userVector.size() = " << this->fdMap.size() << std::endl;
}

void	Server::_delUser(User &user)
{
	if (user.isRegistered())
		this->userMap.erase(user.getNick());
	this->fdMap.erase(user.getFd());
	this->pollfds[user.getPollIndex()].fd = 0;
//	this->userVector.erase(std::remove(this->userVector.begin(), this->userVector.end(), &user), this->userVector.end());
	std::cerr << "Server::_delUser() userVector.size() = " << this->fdMap.size() << std::endl;
	this->sendTo("User <anonymous> disconnect");
	delete &user;
}

int	Server::checkUserConnection(void)
{
	User	*user;

	if (this->pollfds[0].revents & POLLIN)
	{
		try
		{
			user = &this->_accept();
			user->setSignTime(time(NULL));
			if (user->sendTo("Hello\r\n") <= 0)
			{
				std::cerr << "Server::checkUserConnection function user->sendTo() failed" << std::endl;
				exit(EXIT_FAILURE);
			}
			this->_addUser(*user);
			this->sendTo("New user is connected.\r\n");
			return (1);
		}
		catch (Server::ServerFullException &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	return (0);
}

void	Server::checkUserInput(void)
{
	int		size;
	char	buffer[BUFFERSIZE + 1];
	User	*user;

	for (int i = 1; i < MAXUSERS + 1; i++)
	{
		if (this->pollfds[i].revents & POLLIN)
		{
			user = this->fdMap[pollfds[i].fd];
			std::cerr << "hay inforacion de " << i << std::endl;
			size = recv(pollfds[i].fd, buffer, BUFFERSIZE, 0);
			buffer[size] = '\0';
			if (size <= 0)
				this->_delUser(*user);
			else
			{
				this->sendTo(user->getNick() + "> " + buffer);
			}
		}
	}
}

void	Server::loop(void)
{
	int	rv;

	while (!this->stop)
	{
		rv = this->_poll();
		if (rv == -1)
		{
			std::cerr << "Server::loop function poll() failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (rv == 0)
		{
			//timeout
			//check idle to send ping
			//check ping timeout
			//check register timeout
		}
		else
		{
			if (!this->checkUserConnection())
				this->checkUserInput();
		}
	}
}
