#include "ExceptChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"
#include "User.hpp"

#include <ctime>
#include <sstream>

ExceptChanMode::ExceptChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'e';
	this->_chanModeConfig.events = CHANMODE_JOIN | CHANMODE_PRIVMSG | CHANMODE_NOTICE | CHANMODE_NICK;
	this->_chanModeConfig.unique = false;
}

ExceptChanMode::~ExceptChanMode(void)
{}

struct BanInfo
{
	BanInfo(std::string mask, std::string nick)
		: mask(mask), nick(nick), time(std::time(NULL))
	{}

	std::string	mask;
	std::string	nick;
	time_t		time;
};

inline static Channel::Mode::multimap_iterator	findMask(Channel::Mode::rangePairMultimap_type rangePair, const std::string& mask)
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
	Channel*	channel = message.getChannel();
	std::string	mask = message.getSender()->getMask();
	Channel::Mode::rangePairMultimap_type	pairList = channel->mode.getList('e');
	BanInfo		*banInfo;

	(void)event;
	for (; pairList.first != pairList.second; ++pairList.first)
	{
		banInfo = reinterpret_cast<BanInfo*>(pairList.first->second);
		if (banInfo->mask == mask)
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

	//TODO: verificar y completar mascara
	rangePair = channel.mode.getList(this->_chanModeConfig.mode);
	maskIt = findMask(rangePair, message[pos]);
	if (sign && maskIt == rangePair.second)
	{
		banInfo = new BanInfo(message[pos], message.getSender()->getName());
		this->setMode(channel, banInfo);
		return true;
	}
	else if (!sign && maskIt != rangePair.second)
	{
		this->unsetMode(channel, maskIt->second);
		return true;
	}
	return false;
}

void	ExceptChanMode::onShowChanModeEvent(void)
{
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
