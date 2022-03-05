#include "Unknown.hpp"

Unknown::Unknown(Server& server, int fd)
	: ASender(server, fd, time(NULL), LEVEL_UNREGISTERED, TYPE_UNKNOWN, "", "", "", "")
{
	this->_nextTimeout = this->_signTime + REGTIMEOUT;
}

Unknown::~Unknown(void)
{}

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

void	Unknown::_updateMask(void)
{}
