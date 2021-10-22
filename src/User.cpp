#include <map>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "User.hpp"
#include "Server.hpp"

User::User(int fd, Server &server) : server(server), fd(fd)
{}

User::~User(void)
{}

void	User::setHost(std::string value)
{
	this->host = value;
}

std::string const	&User::getHost(void) const
{
	return (this->host);
}

void	User::setIdent(std::string value)
{
	this->ident = value;
}

std::string const &User::getIdent(void) const
{
	return (this->ident);
}

void	User::setReal(std::string value)
{
	this->real = value;
}

std::string const &User::getReal(void) const
{
	return (this->real);
}

void	User::setSignTime(time_t value)
{
	this->signTime = value;
}

time_t const	&User::getSignTime(void) const
{
	return (this->signTime);
}

void	User::setNick(std::string value)
{
	this->nick = value;
}

std::string const	&User::getNick(void) const
{
	return (this->nick);
}

void	User::setPollIndex(int value)
{
	this->pollIndex = value;
}

int	const	&User::getPollIndex(void) const
{
	return (this->pollIndex);
}

void	User::setAwayMsg(std::string value)
{
	this->awayMsg = value;
}

std::string const	&User::getAwayMsg(void) const
{
	return (this->awayMsg);
}

void	User::setFd(int value)
{
	this->fd = value;
}

int const &User::getFd(void) const
{
	return (this->fd);
}

void	User::setRegistered(bool value)
{
	this->registered = value;
}

bool const	&User::isRegistered(void) const
{
	return(this->registered);
}

std::map<std::string, Channel *> &User::getChannels(void)
{
	return (this->channels);
}

Server const	&User::getServer(void) const
{
	return (this->server);
}

bool	User::isAway(void)
{
	return (!this->awayMsg.empty());
}

ssize_t	User::sendTo(std::string msg)
{
	return (send(this->fd, msg.c_str(), msg.size(), 0));
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
