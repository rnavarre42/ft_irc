#include "Channel.hpp"
#include "AChanMode.hpp"
#include "utils.hpp"
#include "Console.hpp"
#include "User.hpp"

#include <string>
#include <map>
#include <ctime>

Channel::Channel(std::string name, User &user, Server &server)
	: _name(name)
	, _owner(user.getName())
	, _server(server)
{}

Channel::~Channel(void)
{
	for (Channel::Mode::multimap_iterator it = this->mode.begin(); it != this->mode.end(); ++it)
		this->_server.findChanMode(it->first)->onDelete(it->second);
}

void *Channel::Mode::operator[](char chr)
{
	return this->_modeMultimap.find(chr)->second;
}

Channel::Mode::multimap_iterator	Channel::Mode::begin(void)
{
	return this->_modeMultimap.begin();
}

Channel::Mode::multimap_iterator	Channel::Mode::end(void)
{
	return this->_modeMultimap.end();
}

bool Channel::Mode::isSet(char modeName)
{
	return this->_modeMultimap.count(modeName) > 0;
}

bool Channel::Mode::isSet(char modeName, void *value)
{
	return (this->findUnique(modeName, value) != this->_modeMultimap.end());
}

Channel::Mode::rangePairMultimap_type Channel::Mode::getList(char modeName)
{
	return this->_modeMultimap.equal_range(modeName);
}

bool Channel::Mode::insert(char modeName, void *value)
{
	bool	ret = !this->isSet(modeName, value);

	if (ret)
		this->_modeMultimap.insert(std::make_pair(modeName, value));
	return ret;
}

Channel::Mode::multimap_iterator	Channel::Mode::findUnique(char modeName, void *value)
{
	for (Channel::Mode::rangePairMultimap_type rangePair = this->getList(modeName);
			rangePair.first != rangePair.second;
			++rangePair.first)
	{
		if (rangePair.first->second == value)
		{
			return rangePair.first;
			break ;
		}
	}
	return this->_modeMultimap.end();
}

bool	Channel::Mode::erase(char modeName, void *value)
{
	Channel::Mode::multimap_iterator	pos = this->findUnique(modeName, value);
	   
	if (pos	!= this->_modeMultimap.end())
	{
		this->erase(pos);
		return true;
	}
	return false;
}

bool	Channel::Mode::erase(char modeName)
{
	return this->_modeMultimap.erase(modeName) > 0;
}

void	Channel::Mode::erase(Channel::Mode::multimap_iterator pos)
{
	this->_modeMultimap.erase(pos);
}

bool Channel::isOper(ISender *sender)
{
	return (this->mode.isSet('o', sender));
//	Channel::Mode::rangePairMultimap_type		rangeIt = mode.getList('o');
//	for (Channel::Mode::multimap_iterator	it = rangeIt.first; it != rangeIt.second; it++)
//	{
//		if (it->second == user)
//			return true;
//	}
//	return false;
}

bool Channel::isVoice(ISender *sender)
{
	return (this->mode.isSet('v', sender));
}

std::string const	&Channel::getName(void) const
{
	return this->_name;
}

Server::userMap_insert	Channel::insert(User *user)
{
	return this->_userMap.insert(std::make_pair(strToUpper(user->getName()), user));
}

void	Channel::erase(User *user)
{
	this->_userMap.erase(strToUpper(user->getName()));
}
/*
Server::userMap_type	&Channel::getUserMap(void)
{
	return this->_userMap;
}
*/

bool	Channel::empty(void)
{
	return this->_userMap.empty();
}

void Channel::setOwner(std::string value)
{
	this->_owner = value;
}

std::string const	&Channel::getOwner(void) const
{
	return this->_owner;
}

void Channel::setTopicInfo(std::string own, std::string topic)
{
	this->_topicInfo.setTopic(own, topic);
}

Channel::TopicInfo const	&Channel::getTopicInfo(void) const
{
	return this->_topicInfo;
}

void Channel::send(std::string msg)
{
	for (Server::userMap_iterator it = this->_userMap.begin(); it != this->_userMap.end(); it++)
		it->second->send(msg);
}

void Channel::join(User user)
{
	Server::userMap_iterator	it;

	it = this->_userMap.find(user.getName());
	if (it == this->_userMap.end())
	{
		this->send(user.getName() + " ha entrado al canal " + this->_name + "\r\n");
		this->_userMap.insert(std::make_pair(user.getName(), &user));
		user.insert(this);
		//user.getChannelMap()[this->_name] = this;
		user.send("Has entrado al canal " + this->_name + "\r\n");
	}
	else
		user.send("Ya estas dentro de " + this->_name + "\r\n");
}

void Channel::part(User user)
{
	Server::userMap_iterator	it;

	it = this->_userMap.find(user.getName());
	if (it == this->_userMap.end())
		user.send("No estas dentro de " + this->_name + "\r\n");
	else
	{
		user.send("Has salido de " + this->_name + "\r\n");
		this->_userMap.erase(it);
		user.erase(this);
		this->send(user.getName() + " ha salido de " + this->_name + "\r\n");
	}
}
