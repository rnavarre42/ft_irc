#include "User.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Console.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include <map>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>

User::User(int fd, Server &server) :
	_server(server), 
	_registered(false), 
	_signTime(time(NULL)), 
	_nextTimeout(this->_signTime + REGTIMEOUT), 
	_fd(fd), 
	_type(TYPE_USER)
{}

User::~User(void)
{
	if (this->isRegistered())
		Console::log(LOG_INFO, "User <" + this->_name + "> disconnected");
	else
		Console::log(LOG_INFO, "User <anonymous> disconnected");
	close(this->_fd);
//	for (Server::channelMap_iterator it = this->_channelMap.begin(); it != this->_channelMap.end(); it++)
//		it->second->getUserMap().erase(strToUpper(this->_name));
	this->_channelMap.clear();
	//this->setRegistered(false);
	this->_fd = 0;
}

void	User::setHost(std::string value)
{
	this->_host = value;
}

std::string const	&User::getHost(void) const
{
	return this->_host;
}

void	User::setIdent(std::string value)
{
	this->_ident = value;
}

std::string const &User::getIdent(void) const
{
	return this->_ident;
}

void	User::setReal(std::string value)
{
	this->_real = value;
}

std::string const &User::getReal(void) const
{
	return this->_real;
}

std::string &User::getInputBuffer(void)
{
	return this->_inputBuffer;
}

std::string &User::getOutputBuffer(void)
{
	return this->_outputBuffer;
}

void	User::setSignTime(time_t value)
{
	this->_signTime = value;
}

time_t const	&User::getSignTime(void) const
{
	return this->_signTime;
}

void	User::setName(std::string value)
{
	this->_name = value;
}

std::string const	&User::getName(void) const
{
	return this->_name;
}

void	User::setPass(std::string value)
{
	this->_pass = value;
}

std::string			User::getMask(void)
{
	return this->_name + "!" + this->_ident + "@" + this->_host;
}

std::string const	&User::getPass(void) const
{
	return this->_pass;
}

bool	User::isUser(void)
{
	return this->_type == TYPE_USER;
}

bool	User::isServer(void)
{
	return this->_type == TYPE_SERVER;
}

bool	User::isOper(void)
{
	//TODO hay que extraer el modo de usuario +O
	return false;
}

int		User::getType(void)
{
	return this->_type;
}

void	User::setPollIndex(int value)
{
	this->_pollIndex = value;
}

int	const	&User::getPollIndex(void) const
{
	return this->_pollIndex;
}

void	User::setAwayMsg(std::string value)
{
	this->_awayMsg = value;
}

std::string const	&User::getAwayMsg(void) const
{
	return this->_awayMsg;
}

void	User::setIdleTime(time_t value)
{
	this->_idleTime = value;
}

time_t const	&User::getIdleTime(void) const
{
	return this->_idleTime;
}

void	User::setNextTimeout(time_t value)
{
	this->_nextTimeout = value;
}

time_t const	&User::getNextTimeout(void) const
{
//	std::cout << "User::getNextTimeout = " << this->nextTimeout << " - " << time(NULL) << " used" << std::endl;
	return this->_nextTimeout;
}

void	User::setFd(int value)
{
	this->_fd = value;
}

int const &User::getFd(void) const
{
	return this->_fd;
}

void	User::clearPingChallenge(void)
{
	this->_pingChallenge.clear();
}

void	User::setPingChallenge(std::string value)
{
	this->_pingChallenge = value;
}

std::string const	&User::getPingChallenge(void) const
{
	return this->_pingChallenge;
}

void	User::setRegistered(bool value)
{
//	std::cout << "User::setRegisterd used" << std::endl;
	this->_registered = value;
}

bool const	&User::isRegistered(void) const
{
	return(this->_registered);
}

std::map<std::string, Channel *> &User::getChannelMap(void)
{
	return this->_channelMap;
}

Server const	&User::getServer(void) const
{
	return this->_server;
}

bool	User::isAway(void)
{
	return !this->_awayMsg.empty();
}

void	User::delChannel(std::string value)
{
	this->_channelMap.erase(strToUpper(value));
}

void	User::sendToBuffer(Message &message)
{
	this->sendToBuffer(message.toString());
}

void	User::sendToBuffer(std::string msg)
{
	if (msg.size() > (MAXLINE - 2))
		msg.erase((MAXLINE - 2), std::string::npos);
	this->_outputBuffer += msg + "\r\n";
}

ssize_t	User::send(std::string msg)
{
	ssize_t	len;

	if (!msg.empty())
		this->sendToBuffer(msg);
	len = ::send(this->_fd, this->_outputBuffer.c_str(), this->_outputBuffer.size(), 0);
	if ((size_t)len != msg.size())
	{
		this->_outputBuffer.erase(0, len);
		this->_server.setPollout(*this);
	}
	else
		this->_outputBuffer.clear();
	return len;
}

ssize_t	User::send(Message &message)
{
	this->sendToBuffer(message.toString());
	return this->send();
}

size_t	User::recv(int fd)
{
	size_t	size;
	char	buffer[BUFFERSIZE + 1];

	size = ::recv(fd, buffer, BUFFERSIZE, 0);
	buffer[size] = '\0';
	this->_inputBuffer += buffer;
	return size;
}

Message	*User::buildMessage(std::string &buff)
{
	return &Message::builder(*this, buff);
}

std::string	User::_getLine(size_t pos)
{
	std::string	line;

	line = this->_inputBuffer.substr(0, pos);
	this->_inputBuffer.erase(0, pos + 1);
	while ((pos = line.find('\r')) != std::string::npos)
		line.erase(pos, 1);
	return line;
}

size_t	User::checkInput(int fd)
{
	size_t		size;
	size_t		pos;
	std::string	lineBuffer;
	Message		*msg;

	size = this->recv(fd);
	while ((pos = this->_inputBuffer.find('\n')) != std::string::npos)
	{
		lineBuffer = this->_getLine(pos);
		msg = this->buildMessage(lineBuffer);
		if (!msg->empty() && !this->_server.recvCommand(*msg))
		{
			Numeric::insertField(msg->getCmd());
			this->send(Numeric::builder(this->_server, *this, ERR_UNKNOWNCOMMAND));
		}
		delete msg;
	}
	return size;
}

bool	User::checkOutput(int fd)
{
	size_t	size;

	size = ::send(fd, this->_outputBuffer.c_str(), this->_outputBuffer.size(), 0);
	if (size == this->_outputBuffer.size())
	{
		this->_outputBuffer.clear();
		return true;
	}
	this->_outputBuffer.erase(0, size);
	return false;
}
