#include "Channel.hpp"
#include "AChanMode.hpp"
#include "utils.hpp"
#include "Console.hpp"
#include "User.hpp"

#include <string>
#include <map>
#include <ctime>

Channel::Channel(const std::string& name, const User& user, Server& server)
	: _name(name)
	, _owner(user.getName())
	, _server(server)
{}

Channel::~Channel(void)
{
	for (Channel::Mode::multimap_iterator it = this->mode.begin(); it != this->mode.end(); ++it)
		this->_server.findChanMode(it->first)->onDelete(it->second);
}

void*&	Channel::Mode::operator[](char chr)
{
	Channel::Mode::multimap_iterator pos;

	if ((pos = this->_modeMultimap.find(chr)) == this->_modeMultimap.end())
		return this->_modeMultimap.insert(std::make_pair(chr, static_cast<void*>(0)))->second;
	return pos->second;
}

Channel::Mode::multimap_iterator	Channel::Mode::find(char chr)
{
	return this->_modeMultimap.find(chr);
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

bool Channel::Mode::isSet(const char modeName, const void* value)
{
	return (this->findUnique(modeName, value) != this->_modeMultimap.end());
}

Channel::Mode::rangePairMultimap_type Channel::Mode::getList(const char modeName)
{
	return this->_modeMultimap.equal_range(modeName);
}

bool Channel::Mode::insert(char modeName, void* value)
{
	bool	ret = !this->isSet(modeName, value);

	if (ret)
		this->_modeMultimap.insert(std::make_pair(modeName, value));
	return ret;
}

Channel::Mode::multimap_iterator	Channel::Mode::findUnique(const char modeName, const void* value)
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

bool	Channel::Mode::erase(const char modeName, const void* value)
{
	Channel::Mode::multimap_iterator	pos = this->findUnique(modeName, value);
	   
	if (pos	!= this->_modeMultimap.end())
	{
		this->erase(pos);
		return true;
	}
	return false;
}

bool	Channel::Mode::erase(const char modeName)
{
	return this->_modeMultimap.erase(modeName) > 0;
}

void	Channel::Mode::erase(Channel::Mode::multimap_iterator pos)
{
	this->_modeMultimap.erase(pos);
}

bool Channel::isOper(const ASender& sender)
{
	return (this->mode.isSet('o', &sender));
}

bool Channel::isVoice(const ASender& sender)
{
	return (this->mode.isSet('v', &sender));
}

const std::string&	Channel::getName(void) const
{
	return this->_name;
}

Server::userMap_insert	Channel::insert(User* user)
{
	return this->_userMap.insert(std::make_pair(strToUpper(user->getName()), user));
}

Server::userMap_insert	Channel::insert(const std::string& userName, User* user)
{
	return this->_userMap.insert(std::make_pair(strToUpper(userName), user));
}

void	Channel::erase(const User* user)
{
	this->_userMap.erase(strToUpper(user->getName()));
}

void	Channel::erase(const std::string& userName)
{
	this->_userMap.erase(strToUpper(userName));
}

bool	Channel::empty(void)
{
	return this->_userMap.empty();
}

void Channel::setOwner(const std::string& value)
{
	this->_owner = value;
}

const std::string&	Channel::getOwner(void) const
{
	return this->_owner;
}

void Channel::setTopicInfo(const std::string& own, const std::string& topic)
{
	this->_topicInfo.setTopic(own, topic);
}

const Channel::TopicInfo&	Channel::getTopicInfo(void) const
{
	return this->_topicInfo;
}

void Channel::send(std::string msg)
{
	for (Server::userMap_iterator it = this->_userMap.begin(); it != this->_userMap.end(); it++)
		it->second->send(msg);
}
