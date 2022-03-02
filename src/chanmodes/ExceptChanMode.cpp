#include "ExceptChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "BanInfo.hpp"
#include "utils.hpp"

#include <ctime>
#include <sstream>

ExceptChanMode::ExceptChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'e';
	this->_chanModeConfig.events = COMMAND_JOIN | COMMAND_PRIVMSG | COMMAND_NOTICE | COMMAND_NICK;
	this->_chanModeConfig.unique = false;
}

ExceptChanMode::~ExceptChanMode(void)
{}

Channel::Mode::multimap_iterator	findMask(Channel::Mode::rangePairMultimap_type rangePair, const std::string& mask)
{
	for (; rangePair.first != rangePair.second; ++rangePair.first)
	{
		if (static_cast<BanInfo* >(rangePair.first->second)->mask == mask)
			return rangePair.first;
	}
	return rangePair.second;
}

void	ExceptChanMode::onChanEvent(Access& access, int event, Message& message, int&)
{
	Channel*								channel = message.getChannel();
	std::string								mask = message.getSender()->getMask();
	Channel::Mode::rangePairMultimap_type	pairList = channel->mode.getList('e');
	BanInfo*								banInfo;

	if (event & (COMMAND_PRIVMSG | COMMAND_NOTICE) && channel->mode.isSet('m'))
		return ;
	for (; pairList.first != pairList.second; ++pairList.first)
	{
		banInfo = reinterpret_cast<BanInfo*>(pairList.first->second);
		if (*banInfo == mask)
		{
			access = AChanMode::allow;
			break ;
		}
	}
}

bool	ExceptChanMode::onChanModeEvent(int pos, int sign, Channel& channel, Message& message)
{
	BanInfo*								banInfo;
	Channel::Mode::rangePairMultimap_type	rangePair;
	Channel::Mode::multimap_iterator		maskIt;
	std::string								newMask;

	newMask = addWildcard(message[pos]);
	rangePair = channel.mode.getList(this->_chanModeConfig.mode);
	maskIt = findMask(rangePair, newMask);
	if (sign && maskIt == rangePair.second)
	{
		banInfo = new BanInfo(newMask, message.getSender()->getName());
		message[pos] = newMask;
		this->setMode(channel, banInfo);
		return true;
	}
	else if (!sign && maskIt != rangePair.second)
	{
		message[pos] = newMask;
		this->unsetMode(channel, maskIt->second);
		return true;
	}
	return false;
}

void	ExceptChanMode::onShowChanModeEvent(Channel& channel, Message& message)
{
	Channel::Mode::rangePairMultimap_type	it = channel.mode.getList(this->_chanModeConfig.mode);

	for ((it = channel.mode.getList(this->_chanModeConfig.mode))
			; it.first != it.second
			; ++it.first)
	{
		Numeric::insertField(channel.getName());
		Numeric::insertField(this->toString(it.first->second));
		message.replyNumeric(RPL_EXCEPTLIST);
	}
	Numeric::insertField(channel.getName());
	message.replyNumeric(RPL_ENDOFEXCEPTLIST);
}

void	ExceptChanMode::onDelete(void* pointer)
{
	delete &*reinterpret_cast<BanInfo* >(pointer);
}

std::string ExceptChanMode::toString(void* pointer)
{
	std::ostringstream	oss;
	BanInfo*			banInfo;

	banInfo	= reinterpret_cast<BanInfo* >(pointer);
	oss << banInfo->mask << " " << banInfo->nick << " :" << banInfo->time;
	return oss.str();
}
