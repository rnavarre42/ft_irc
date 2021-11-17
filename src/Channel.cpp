#include "Channel.hpp"
#include "utils.hpp"
#include <string>
#include <map>
#include <ctime>

Channel::Channel(std::string name, User &user) : _name(name), _owner(user.getName())
{}

Channel::Channel(void)
{}

Channel::~Channel(void)
{}

std::string	const &Channel::getName(void) const
{
	return this->_name;
}

std::map<std::string, User *>	&Channel::getUserMap(void)
{
	return this->_userMap;
}

void Channel::delUser(std::string value)
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
	for (std::map<std::string, User *>::iterator it = this->_userMap.begin(); it != this->_userMap.end(); it++)
		it->second->send(msg);
}

void Channel::join(User user)
{
	std::map<std::string, User *>::iterator it;

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
	std::map<std::string, User *>::iterator it;

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
