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

User::User(Server& server, int fd)
	: ASender(server, fd, time(NULL), LEVEL_UNREGISTERED, TYPE_LOCALUSER, "", "", "", "")
	, _ident("anonymous")
{}

User::User(Server& server, time_t signTime, const std::string& pass, const std::string& name, const std::string& ident, const std::string& host, const std::string& real)
	: ASender(server, -1, signTime, LEVEL_REGISTERED, TYPE_REMOTEUSER, pass, name, host, real)
	, _ident(ident)
{
	this->_updateMask();
}

User::~User(void)
{
	close(this->_fd);
	this->_channelMap.clear();
	this->_fd = 0;
}

User::User(const Unknown& src)
	: ASender(src._server, src._fd, src._signTime, src._level, TYPE_LOCALUSER, src._pass, src._name, src._host, src._real)
{
	this->_nextTimeout = src._nextTimeout;
	this->_idleInnerTime = time(NULL);
	this->_updateMask();

}

bool	User::isOnChannel(const std::string& channelName)
{
	return (this->find(channelName) != this->_channelMap.end());
}

bool	User::isOnChannel(const Channel& channel)
{
	return (this->find(channel.getName()) != this->_channelMap.end());
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

bool	User::isOper(void)
{
	return this->_oper;
}

void	User::enableOper(void)
{
	this->_oper = true;
}

void	User::setAwayMsg(const std::string& value)
{
	this->_awayMsg = value;
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

void	User::clearPingChallenge(void)
{
	this->_pingChallenge.clear();
}

void	User::insert(Channel* channel)
{
	this->_channelMap.insert(std::make_pair(strToUpper(channel->getName()), channel));
}

void	User::erase(Channel* channel)
{
	this->_channelMap.erase(strToUpper(channel->getName()));
}

bool	User::isAway(void)
{
	return !this->_awayMsg.empty();
}

void	User::_updateMask(void)
{
	this->_mask = this->_name + "!" + this->_ident + "@" + this->_host;
}

ssize_t	User::send(void)
{
	return this->send("");
}

ssize_t	User::send(const Message& message)
{
	this->sendToBuffer(message.toString());
	return this->send();
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
