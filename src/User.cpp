#include "IrcServer.hpp"
#include <map>
#include <iostream>
#include <string>
#include <unistd.h>

User::User(int fd, Server &server) : fd(fd), server(server)
{}

User::User(void)
{}

User::~User(void)
{}

bool	User::isAway(void)
{
	return (!this->awayMsg.empty());
}

int		User::send(std::string msg)
{
	return (send(this->fd, msg, msg.size(), 0);
}

void	User::disconnect(void)
{
	if (this->isRegistered())
		std::cout << "Client <" << this->nick << "> close connection" << std::endl;
	else
		std::cout << "Client <anonymous> close connection" << std::endl;
	close(this->fd);
	this->fd = 0;
}
