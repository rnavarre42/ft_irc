#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"

Server::Server(std::string ip, int port)
	: ip(ip), port(port)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, this->signalHandler);
	stop = false;
	initSocket();
	bind();
}

Server::~Server(void)
{}

void	Server::signalHandler(int sig)
{
	if (sig == SIGINT)
	{
		log(LOG_CONNECT, "Disconnecting clients...");
		this->closeUsers("Shutdown. Please, reconnect to another server.");
		this->stop = true;
	}
}

void	Server::send(std::string msg)
{
	for (User user : this->users)
		user.send(msg);
}

void	Server::quit(std::string msg)
{
	for (User user : this->users)
	{
		user.send(msg);
		user.disconnect();
	}
}

void	Server::start(void)
{
	listen();
	this->loop();
}

void	Server::initSocket(void)
{
	this->fd = socket(AF_INET SOCK_STREAM, 0);
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
		if (pollfds.fd[i] == 0)
			return (i);
	}
	return (0);
}

User	&Server::accept(void)
{
	User	user;
	int		newFd;

	newFd = accept(this->fd, (struct sockaddr *)&this->address, (socklen_t *)&this->addrlen);
	if (newFd < 0)
	{
		std::cerr << "Server::accept function accept() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (users.count() == MAXUSERS)
	{
		user.send("The server is full. Please, try again more later.\r\n");
		close(newFd);
		return (NULL);
	}
	user.setFd(newFd);
	user.setPollIndex(findFreePollIndex());
	pollfds[user.getPollIndex()].fd = newFd;
	pollfds[user.getPollIndex()].events = POLLIN;
	return (&user);
}

void	Server::bind(void)
{
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->port);
	if (bind(this->fd, (struct sockaddr *)&this->address, sizeof(server->address)) < 0)
	{
		std::cerr << "Server::bind function bind() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::listen(void)
{
	if (listen(this->fd, MAXLISTENQUEUE) < 0)
	{
		std::cerr << "Server::listen function listen() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	this->pollfds[0].fd = this->fd;
	this->pollfds[0].events = POLLIN;
}

int		Server::pool(void)
{
	return (poll(this->pollfds, this->clients.count() + 1, this->timeout));
}

void	Server::loop(void)
{
	int	rv;

	while (!stop)
	{
		rv = poll();
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
