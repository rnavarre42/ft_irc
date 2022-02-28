#include "Unknown.hpp"

Unknown::Unknown(int fd, Server& server)
	: _server(server)
	, _ident("anonymous")
	, _signTime(time(NULL))
	, _nextTimeout(this->_signTime + REGTIMEOUT)
	, _status(1)
	, _fd(fd)
	, _type(TYPE_UNKNOWN)
{}

Unknown::~Unknown(void)
{}

const std::string&	Unknown::getName(void) const
{
	return this->_name;
}

const int&	Unknown::getFd(void) const
{
	return this->_fd;
}

const std::string&	Unknown::getPass(void) const
{
	return this->_pass;
}

void	Unknown::setPass(const std::string& value)
{
	this->_pass = value;
}

ssize_t	Unknown::send(void)
{
	return 0;
}

ssize_t	Unknown::send(const std::string&)
{
	return 0;
}

ssize_t	Unknown::send(const Message&)
{
	return 0;
}

void	Unknown::setStatus(int value)
{
	this->_status = value;
}

int	Unknown::getStatus(void)
{
	return this->_status;
}

bool	Unknown::isServer(void)
{
	return false;
}

bool	Unknown::isUser(void)
{
	return false;
}

bool	Unknown::isOper(void)
{
	return false;
}

int	Unknown::getType(void)
{
	return this->_type;
}

const std::string&	Unknown::getMask(void) const
{
	return this->_mask;
}

