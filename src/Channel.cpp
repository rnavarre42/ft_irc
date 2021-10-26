#include <string>
#include <map>
#include <ctime>
#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(std::string name) : name(name)
{}

Channel::Channel(void)
{}

std::string	const &Channel::getName(void) const
{
	return (this->name);
}

std::map<std::string, User *> const	&Channel::getUserMap(void) const
{
	return (this->userMap);
}

void Channel::setOwner(std::string value)
{
	this->owner = value;
}

std::string const	&Channel::getOwner(void) const
{
	return (this->owner);
}

void Channel::setTopic(std::string value)
{
	this->topic = value;
}

std::string const	&Channel::getTopic(void) const
{
	return (this->topic);
}

void Channel::setTopicOwn(std::string value)
{
	this->topicOwn = value;
}

std::string const	&Channel::getTopicOwn(void) const
{
	return (this->topicOwn);
}

void Channel::setTopicTime(time_t value)
{
	this->topicTime = value;
}

time_t const	&Channel::getTopicTime(void) const
{
	return (this->topicTime);
}

void Channel::send(std::string msg)
{
	for (std::map<std::string, User *>::iterator it = this->userMap.begin(); it != this->userMap.end(); it++)
		it->second->send(msg);
}

void Channel::join(User user)
{
	std::map<std::string, User *>::iterator it;

	it = this->userMap.find(user.getNick());
	if (it == this->userMap.end())
	{
		this->send(user.getNick() + " ha entrado al canal " + this->name + "\r\n");
		this->userMap[user.getNick()] = &user;
		user.getChannelMap()[this->name] = this;
		user.send("Has entrado al canal " + this->name + "\r\n");
	}
	else
		user.send("Ya estas dentro de " + this->name + "\r\n");
}

void Channel::part(User user)
{
	std::map<std::string, User *>::iterator it;

	it = this->userMap.find(user.getNick());
	if (it == this->userMap.end())
		user.send("No estas dentro de " + this->name + "\r\n");
	else
	{
		user.send("Has salido de " + this->name + "\r\n");
		this->userMap.erase(it);
		user.getChannelMap().erase(this->name);
		this->send(user.getNick() + " ha salido de " + this->name + "\r\n");
	}
}
