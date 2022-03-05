#include "ASender.hpp"
#include "config.hpp"
#include "Message.hpp"
#include "Numeric.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <string>

ASender::ASender(void)
	: _server(*dynamic_cast<Server*>(this))
	, _type(TYPE_LOCALSERVER)
{}

ASender::ASender(Server& server
		, int fd
		, time_t signTime
		, int level
		, int type
		, const std::string& pass
		, const std::string& name
		, const std::string& host
		, const std::string& real)
	: _server(server)
	, _fd(fd)
	, _signTime(signTime)
	, _level(level)
	, _type(type)
	, _pass(pass)
	, _name(name)
	, _host(host)
	, _real(real)
{
	if (this->_type == TYPE_LOCALUSER)
	{
		this->_nextTimeout = this->_signTime + REGTIMEOUT;
		this->_level = LEVEL_UNREGISTERED;
	}
	this->_doUpdateMask();
}

ASender::~ASender(void)
{}

void	ASender::setPass(const std::string& value)
{
	this->_pass = value;
}

const std::string&	ASender::getPass(void) const
{
	return this->_pass;
}

void	ASender::setName(const std::string& value)
{
	this->_name = value;
	this->_updateMask();
}

const std::string&	ASender::getName(void) const
{
	return this->_name;
}

void	ASender::setHost(const std::string& value)
{
	this->_host = value;
	this->_updateMask();
}

const	std::string&	ASender::getHost(void) const
{
	return this->_host;
}

void	ASender::setReal(const std::string& value)
{
	this->_real = value;
}

const std::string&	 ASender::getReal(void) const
{
	return this->_real;
}

void	ASender::setLevel(int value)
{
	this->_level = value;
}

int	ASender::getLevel(void)
{
	return this->_level;
}

int	ASender::getFd(void) const
{
	return this->_fd;
}

const std::string&	ASender::getMask(void) const
{
	return this->_mask;
}

int	ASender::getType(void) const
{
	return this->_type;
}

void	ASender::setNextTimeout(time_t value)
{
	this->_nextTimeout = value;
}

time_t	ASender::getNextTimeout(void) const
{
	return this->_nextTimeout;
}

void	ASender::setIdleTime(time_t value)
{
	this->_idleTime = value;
}

time_t	ASender::getIdleTime(void) const
{
	return this->_idleTime;
}

void	ASender::setIdleInnerTime(time_t value)
{
	this->_idleInnerTime = value;
}

time_t	ASender::getIdleInnerTime(void) const
{
	return this->_idleInnerTime;
}

void	ASender::setPingChallenge(const	std::string& value)
{
	this->_pingChallenge = value;
}

const std::string&	ASender::getPingChallenge(void) const
{
	return this->_pingChallenge;
}


size_t	ASender::recv(void)
{
	size_t	size;
	char	buffer[BUFFERSIZE + 1];

	size = ::recv(this->_fd, buffer, BUFFERSIZE, 0);
	buffer[size] = '\0';
	this->_inputBuffer += buffer;
	return size;
}

size_t	ASender::checkInput(Message& message)
{
	size_t		size;
	size_t		pos;
	std::string	lineBuffer;

	size = this->recv();
	while ((pos = this->_inputBuffer.find('\n')) != std::string::npos)
	{
		lineBuffer = this->_getLine(pos);
		message.set(this, lineBuffer);
		if (!message.empty() && !this->_server.recvCommand(message))
		{
			Numeric::insertField(message.getCmd());
			this->send(Numeric::builder(this->_server, *this, ERR_UNKNOWNCOMMAND));
		}
		message.clear();
	}
	return size;
}

bool	ASender::checkOutput(void)
{
	size_t	size;

	size = ::send(this->_fd, this->_outputBuffer.c_str(), this->_outputBuffer.size(), 0);
	if (size == this->_outputBuffer.size())
	{
		this->_outputBuffer.clear();
		return true;
	}
	this->_outputBuffer.erase(0, size);
	return false;
}

std::string	ASender::_getLine(size_t pos)
{
	std::string	line;

	line = this->_inputBuffer.substr(0, pos);
	this->_inputBuffer.erase(0, pos + 1);
	while ((pos = line.find('\r')) != std::string::npos)
		line.erase(pos, 1);
	return line;
}

void	ASender::_doUpdateMask(void)
{
	this->_updateMask();
}
