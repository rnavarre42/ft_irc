#include "Unknown.hpp"
#include "Message.hpp"

Unknown::Unknown(Server& server, int fd)
	: ASender(server, fd, time(NULL), LEVEL_UNREGISTERED, TYPE_UNKNOWN, "", "", "", "")
{
	this->_nextTimeout = this->_signTime + REGTIMEOUT;
}

Unknown::~Unknown(void)
{}

ssize_t	Unknown::send(void)
{
	return this->send("");
}

ssize_t	Unknown::send(const std::string& data)
{
	ssize_t	len;

	if (!data.empty())
		this->sendToBuffer(data);
	len = ::send(this->_fd, this->_outputBuffer.c_str(), this->_outputBuffer.size(), 0);
	if (static_cast<size_t>(len) != data.size())
	{
		this->_outputBuffer.erase(0, len);
		this->_server.setPollout(*this);
	}
	else
		this->_outputBuffer.clear();
	return len;
}

ssize_t	Unknown::send(const Message& message)
{
	this->sendToBuffer(message.toString());
	return this->send();
}

void	Unknown::_updateMask(void)
{}
