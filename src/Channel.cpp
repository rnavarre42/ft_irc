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

std::map<std::string, User *> const	&Channel::getUsers(void) const
{
	return (this->users);
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

void Channel::sendTo(std::string msg)
{
	for (std::map<std::string, User *>::iterator it = this->users.begin(); it != this->users.end(); it++)
		it->second->sendTo(msg);
}

void Channel::join(User user)
{
	std::map<std::string, User *>::iterator it;

	it = this->users.find(user.getNick());
	if (it == this->users.end())
	{
		this->sendTo(user.getNick() + " ha entrado al canal " + this->name + "\r\n");
		this->users[user.getNick()] = &user;
		user.getChannels()[this->name] = this;
		user.sendTo("Has entrado al canal " + this->name + "\r\n");
	}
	else
		user.sendTo("Ya estas dentro de " + this->name + "\r\n");
}

void Channel::part(User user)
{
	std::map<std::string, User *>::iterator it;

	it = this->users.find(user.getNick());
	if (it == this->users.end())
		user.sendTo("No estas dentro de " + this->name + "\r\n");
	else
	{
		user.sendTo("Has salido de " + this->name + "\r\n");
		this->users.erase(it);
		user.getChannels().erase(this->name);
		this->sendTo(user.getNick() + " ha salido de " + this->name + "\r\n");
	}
}
