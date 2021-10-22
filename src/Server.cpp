#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <exception>
#include "Server.hpp"
#include "User.hpp"

Server	*Server::instance = nullptr;

void	signalHandler(int sig)
{
	Server server = Server::getInstance();

	if (sig == SIGINT)
	{
//		log(LOG_CONNECT, "Disconnecting clients...");
		server.quit("Shutdown. Please, reconnect to another server.");
	}
}

const char	*Server::ServerFullException::what(void) const throw()
{
	return ("The server is full.");
}

Server::Server(std::string ip, int port)
	: ip(ip), port(port)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signalHandler);
	this->stop = false;
	this->initSocket();
	this->timeout = 1000;
	this->_bind();
}

Server::~Server(void)
{}

Server	&Server::getInstance(void)
{
	return (*Server::instance);
}
Server	&Server::getInstance(std::string ip, int port)
{
	if (Server::instance == nullptr)
		Server::instance = new Server(ip, port);
	return (*Server::instance);
}


void	Server::sendTo(std::string msg)
{	
	for (std::map<std::string, User *>::iterator it = users.begin(); it != users.end(); it++)
		it->second->sendTo(msg);
}

void	Server::quit(std::string msg)
{
	for (std::map<std::string, User *>::iterator it = users.begin(); it != users.end(); it++)
	{
		it->second->sendTo(msg);
		it->second->disconnect();
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
	if (this->users.size() == MAXUSERS)
	{
		send(newFd, "The server is full. Please, try again more later.\r\n", 52, 0);
		close(newFd);
		throw std::exception();
	}
	user = new User(newFd, *this);
	user->setPollIndex(findFreePollIndex());
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

void	Server::loop(void)
{
	int	rv;

	while (!this->stop)
	{
		rv = this->_poll();
		if (rv == -1)
		{
			std::cerr << "Server::loop function pool() failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (rv == 0)
		{
			//timeout
			//check idle
			//check ping timeout
			//check register timeout
		}
		else
		{
			//new connections
			//client data
		}
	}
}
