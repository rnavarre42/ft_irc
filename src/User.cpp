#include <map>
#include <iostream>
#include <string>
#include <unistd.h>
#include "User.hpp"
#include "Server.hpp"

User::User(int fd, Server &server) : fd(fd), server(server)
{}

User::User(void)
{}

User::~User(void)
{}

void	setHost(std::string value)
{
	this->host = value;
}

std::string	&getHost(void) const
{
	return (&this->host);
}

void	setIdent(std::string value)
{
	this->ident = value;
}

std::string &getIdent(void) const
{
	return (&this->ident);
}

void	setReal(std::string value)
{
	this->real = value;
}

std::string &getReal(void) const
{
	return (&this->real);
}

void	setSignTime(time_t value)
{
	this->signTime = value;
}

time_t getSignTime(void) const
{
	return (&this->signTime);
}

void	setNick(std::string value)
{
	this->nick = value;
}

std::string &getNick(void) const
{
	return (&this->nick);
}

void	setAwayMsg(std::string value)
{
	this->awayMsg = value;
}

std::string &getAwayMsg(void) const
{
	return (&this->awayMsg);
}

std::map<std::string, Channel *> &getChannels(void) const
{
	return (&this->channels);
}

std::map<std::string, Server *> &getServer(void) const
{
	return (&this->server);
}

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
