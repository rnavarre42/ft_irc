#include <string>
#include <map>
#include <ctime>
#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(std::string name) : name(name)
{}

Channel::Channel(void)
{}

std::string	&getName(void) const
{
	return (&this->name);
}

std::map<std::string, User *> &getUsers(void) const
{
	return (&this->users);
}

void setCreator(std::string value)
{
	this->creator = value;
}

std::string &getCreator(void) const
{
	return (&this->creator);
}

void setTopic(std::string value)
{
	this->topic = value;
}

std::string &getTopic(void) const
{
	return (&this->topic);
}

void setTopicOwn(std::string value)
{
	this->topicOwn = value;
}

std::string &getTopicOwn(void) const
{
	return (&this->topicOwn);
}

void setTopicTime(time_t value)
{
	this->topicTime = value;
}

time_t &getTopicTime(void) const
{
	return (&this->topicTime);
}

void send(std::string msg)
{
	for (User user : this->users)
		user.send(msg);
}

void join(User user)
{
	std::map<std::string, User *>::iterator it;

	it = users.find(user.nick);
	if (it == users.end())
	{
		this->send(user.nick + " ha entrado al canal " + channel.name + "\r\n");
		users.insert(user.name, user);
		user.getChannels.insert(this->name, this);
		user.send("Has entrado al canal " + channel.name + "\r\n");
	}
	else
		user.send("Ya estas dentro de " + channel.name + "\r\n");
}

void part(User user)
{
	std::map<std::string, User *>::iterator it;

	it = users.find(user.nick);
	if (it == users.end())
		user.send("No estas dentro de " + channel.name + "\r\n");
	else
	{
		user.send("Has salido de " + channel.name + "\r\n");
		users.rease(it);
		user.getChannels.erase(this->name);
		this->send(user.nick + " ha salido de " + channel.name + "\r\n");
	}
}
