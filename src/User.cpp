#include "User.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Console.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "AUserMode.hpp"
#include "Unknown.hpp"

#include <map>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>

User::User(int fd, Server& server)
	: _server(server)
	, _ident("anonymous")
	, _status(1)
	, _signTime(time(NULL))
	, _nextTimeout(this->_signTime + REGTIMEOUT)
	, _idleTime(time(NULL))
	, _fd(fd)
	, _type(TYPE_USER)
{}

User::~User(void)
{
	close(this->_fd);
	this->_channelMap.clear();
	this->_fd = 0;
}

User::User(const Unknown& src)
	: _server(src._server)
	, _ident(src._ident)
	, _status(1)
	, _signTime(src._signTime)
	, _nextTimeout(src._nextTimeout)
	, _fd(src._fd)
	, _type(TYPE_USER)
{}

void	User::setHost(const std::string& value)
{
	this->_host = value;
	this->_updateMask();
}

const std::string&	User::getHost(void) const
{
	return this->_host;
}

void	User::setIdent(const std::string& value)
{
	this->_ident = value;
}

const std::string&	User::getIdent(void) const
{
	return this->_ident;
}

void	User::setReal(const std::string& value)
{
	this->_real = value;
	this->_updateMask();
}

const std::string&	User::getReal(void) const
{
	return this->_real;
}

std::string&	User::getInputBuffer(void)
{
	return this->_inputBuffer;
}

bool	User::isOnChannel(const std::string& channelName)
{
	return (this->find(channelName) != this->_channelMap.end());
}

bool	User::isOnChannel(const Channel& channel)
{
	return (this->find(channel.getName()) != this->_channelMap.end());
}

std::string&	User::getOutputBuffer(void)
{
	return this->_outputBuffer;
}

void	User::setSignTime(time_t value)
{
	this->_signTime = value;
}

const time_t&	User::getSignTime(void) const
{
	return this->_signTime;
}

void	User::setName(const std::string& value)
{
	this->_name = value;
	this->_updateMask();
}

const std::string&	User::getName(void) const
{
	return this->_name;
}

void	User::setPass(const std::string& value)
{
	this->_pass = value;
}

void	User::setMode(AUserMode* userMode)
{
	this->_modes |= userMode->getFlag();
}

void	User::unsetMode(AUserMode* userMode)
{
	this->_modes &= ~userMode->getFlag();
}

bool	User::isSetMode(AUserMode* userMode)
{
	return this->_modes & userMode->getFlag();
}

const std::string&	User::getMask(void) const
{
	return this->_mask;
}

const std::string&	User::getPass(void) const
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
	return this->_oper;
}

void	User::setOper(bool value)
{
	this->_oper = value;
}

int		User::getType(void)
{
	return this->_type;
}

void	User::setPollIndex(int value)
{
	this->_pollIndex = value;
}

const int&	User::getPollIndex(void) const
{
	return this->_pollIndex;
}

void	User::setAwayMsg(const std::string& value)
{
	this->_awayMsg = value;
	if (this->_awayMsg.size() > MAXAWAY)
		this->_awayMsg.resize(MAXAWAY);
	this->_awayTime = time(NULL);
}

const std::string&	User::getAwayMsg(void) const
{
	return this->_awayMsg;
}

const time_t&	User::getAwayTime(void) const
{
	return this->_awayTime;
}

void	User::setIdleTime(time_t value)
{
	this->_idleTime = value;
}

const time_t&	User::getIdleTime(void) const
{
	return this->_idleTime;
}

void	User::setNextTimeout(time_t value)
{
	this->_nextTimeout = value;
}

const time_t&	User::getNextTimeout(void) const
{
	return this->_nextTimeout;
}

void	User::setFd(int value)
{
	this->_fd = value;
}

const int&	User::getFd(void) const
{
	return this->_fd;
}

void	User::clearPingChallenge(void)
{
	this->_pingChallenge.clear();
}

void	User::setPingChallenge(const std::string& value)
{
	this->_pingChallenge = value;
}

const std::string&	User::getPingChallenge(void) const
{
	return this->_pingChallenge;
}

void	User::setStatus(int value)
{
	this->_status = value;
}

int	User::getStatus(void)
{
	return this->_status;
}

void	User::insert(Channel* channel)
{
	this->_channelMap.insert(std::make_pair(strToUpper(channel->getName()), channel));
}

void	User::erase(Channel* channel)
{
	this->_channelMap.erase(strToUpper(channel->getName()));
}

const Server&	User::getServer(void) const
{
	return this->_server;
}

bool	User::isAway(void)
{
	return !this->_awayMsg.empty();
}

void	User::_updateMask(void)
{
	this->_mask = this->_name + "!" + this->_ident + "@" + this->_host;
}

void	User::sendToBuffer(const Message& message)
{
	this->sendToBuffer(message.toString());
}

void	User::sendToBuffer(std::string msg)
{

	if (msg.size() > (MAXLINE - 2))
		msg.erase((MAXLINE - 2), std::string::npos);
	this->_outputBuffer += msg + "\r\n";
}

ssize_t	User::send(void)
{
	return this->send("");
}

ssize_t	User::send(const std::string& data)
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

ssize_t	User::send(const Message& message)
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

std::string	User::_getLine(size_t pos)
{
	std::string	line;

	line = this->_inputBuffer.substr(0, pos);
	this->_inputBuffer.erase(0, pos + 1);
	while ((pos = line.find('\r')) != std::string::npos)
		line.erase(pos, 1);
	return line;
}

size_t	User::checkInput(int fd, Message& message)
{
	size_t		size;
	size_t		pos;
	std::string	lineBuffer;

	size = this->recv(fd);
	while ((pos = this->_inputBuffer.find('\n')) != std::string::npos)
	{
		lineBuffer = this->_getLine(pos);
		message.set(*this, lineBuffer);
		if (!message.empty() && !this->_server.recvCommand(message))
		{
			Numeric::insertField(message.getCmd());
			this->send(Numeric::builder(this->_server, *this, ERR_UNKNOWNCOMMAND));
		}
		message.clear();
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

Channel*	User::findFullestChannel(void)
{
	Server::channelMap_iterator		currentIt = this->_channelMap.begin();
	Server::channelMap_iterator		nextIt = currentIt;

	for (; nextIt != this->_channelMap.end(); nextIt++)
	{
		if (nextIt->second->size() > currentIt->second->size())
			currentIt = nextIt;
	}
	return	currentIt->second;
}

Server::userVector_type*	User::getUniqueVector(void)
{
	//aloca memoria para el nuevo vector
	Server::userVector_type*						userVector;
	Server::channelSet_type							checkChannelSet;
	Channel*										currentChannel;
	std::pair<Server::channelSet_iterator, bool>	ret;
	Server::userVector_iterator						vectorIt;

	userVector = new Server::userVector_type;
	if (!this->_channelMap.size())
		return userVector;
	//buscar el canal que tiene mas usuarios
	currentChannel = findFullestChannel();
	//añadir los usuarios del canal mas grande al vector
	checkChannelSet.insert(currentChannel);
	for (Server::userMap_iterator it = currentChannel->begin(); it != currentChannel->end(); it++)
		userVector->push_back(it->second);
	//añadir los usuarios de los canales restantes sin repetir usuario en el vector
	for (Server::channelMap_iterator it = this->_channelMap.begin(); it != this->_channelMap.end(); it++)
	{
		currentChannel = it->second;
		ret = checkChannelSet.insert(currentChannel);
		if (ret.second == true)
			for (Server::userMap_iterator it = currentChannel->begin(); it != currentChannel->end(); it++)
			{
				vectorIt = std::find(userVector->begin(), userVector->end(), it->second);
				if (vectorIt == userVector->end())
					userVector->push_back(it->second);
			}
	}
	return userVector;
}
