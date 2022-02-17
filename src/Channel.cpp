#include "Channel.hpp"
#include "User.hpp"
#include "utils.hpp"
#include "Console.hpp"

#include <string>
#include <map>
#include <ctime>

Channel::Channel(std::string name, User &user) : _name(name), _owner(user.getName())
{}

Channel::Channel(void)
{}

Channel::~Channel(void)
{}

bool Channel::Mode::isSet(char modeName)
{
	return this->_modeMultimap.count(modeName) > 0;
}

Channel::Mode::rangePairMultimap_type Channel::Mode::getList(char modeName)
{
	return this->_modeMultimap.equal_range(modeName);
}

void	Channel::Mode::insert(char modeName, void *value)
{
	Console::log(LOG_DEBUG, "insert mode");
	this->_modeMultimap.insert(std::make_pair(modeName, value));
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

void	Channel::Mode::erase(char modeName, void *value)
{
	Channel::Mode::multimap_iterator	pos = this->findUnique(modeName, value);
	   
	if (pos	!= this->_modeMultimap.end())
		this->erase(pos);
}

void	Channel::Mode::erase(Channel::Mode::multimap_iterator pos)
{
	this->_modeMultimap.erase(pos);
}

bool Channel::isOper(User *user)
{
	Channel::Mode::rangePairMultimap_type		rangeIt = mode.getList('o');
	for (Channel::Mode::multimap_iterator	it = rangeIt.first; it != rangeIt.second; it++)
	{
		if (it->second == user)
			return true;
	}
	return false;
}

std::string const	&Channel::getName(void) const
{
	return this->_name;
}

Server::userMap_type	&Channel::getUserMap(void)
{
	return this->_userMap;
}

void	Channel::insertUser(User *user)
{
	this->_userMap[strToUpper(user->getName())] = user;
}

void	Channel::eraseUser(std::string value)
{
	this->_userMap.erase(strToUpper(value));
}

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

void Channel::setTopic(std::string value)
{
	this->_topic = value;
}

std::string const	&Channel::getTopic(void) const
{
	return this->_topic;
}

void Channel::setTopicOwn(std::string value)
{
	this->_topicOwn = value;
}

std::string const	&Channel::getTopicOwn(void) const
{
	return this->_topicOwn;
}

void Channel::setTopicTime(time_t value)
{
	this->_topicTime = value;
}

time_t const	&Channel::getTopicTime(void) const
{
	return this->_topicTime;
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
		this->_userMap[user.getName()] = &user;
		user.getChannelMap()[this->_name] = this;
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
		user.getChannelMap().erase(this->_name);
		this->send(user.getName() + " ha salido de " + this->_name + "\r\n");
	}
}
