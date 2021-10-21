#include "IrcServer.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server(std::string ip, int port)
	: ip(ip), port(port)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, this->signalHandler);
	stop = false;
}

Server::~Server(void)
{}

void	Server::signalHandler(int sig)
{
	if (sig == SIGINT)
	{
		log(LOG_CONNECT, "Disconnectiong clients...");
		this->closeUsers("Shutdown server. Please, reconnect to another server.");
		this->stop = true;
	}
}

void	Server::sendUsers(std::string msg)
{
	for (User user : this->users)
		user.send(msg);
}

void	Server::closeUsers(std::string msg)
{
	for (User user : this->users)
	{
		user.send(msg);
		user.disconnect();
	}
}

void	Server::start(void)
{
	this->loop();
}

void	Server::Loop(void)
{
	while (!stop)
	{

	}
}
