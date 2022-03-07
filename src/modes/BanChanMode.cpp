#include "BanChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "BanInfo.hpp"
#include "utils.hpp"

#include <ctime>
#include <sstream>

class User;
class Channel;

BanChanMode::BanChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'b';
	this->_chanModeConfig.events = COMMAND_JOIN | COMMAND_PRIVMSG | COMMAND_NOTICE | COMMAND_NICK;
	this->_chanModeConfig.unique = false;
}

BanChanMode::~BanChanMode(void)
{}

void	BanChanMode::onChanEvent(Access& access, int event, Message& message, int& numeric)
{
	Channel&	channel = message.getChannel();
	std::string	mask = message.getSender().getMask();
	BanInfo*	banInfo;

	if (access == AChanMode::deny)
		return ;
	for (Channel::Mode::rangePairMultimap_type pairList = channel.mode.getList('b')
			; pairList.first != pairList.second
			; ++pairList.first)
	{
		banInfo = reinterpret_cast<BanInfo*>(pairList.first->second);
		if (*banInfo == mask)
		{
			Numeric::insertField(channel.getName());
			if (event & COMMAND_NICK)
				numeric = ERR_CANTCHANGENICK;
			if (event & COMMAND_JOIN)
				numeric = ERR_BANNEDFROMCHAN;
			if (event & (COMMAND_PRIVMSG | COMMAND_NOTICE))
				numeric = ERR_CANNOTSENDTOCHAN;
			access = AChanMode::deny;
			break ;
		}
	}
}

Channel::Mode::multimap_iterator	findMask(Channel::Mode::rangePairMultimap_type& rangePair, const std::string& mask)
{
	for (; rangePair.first != rangePair.second; ++rangePair.first)
	{
		if (static_cast<BanInfo *>(rangePair.first->second)->mask == mask)
			return rangePair.first;
	}
	return rangePair.second;
}

bool	BanChanMode::onChanModeEvent(int pos, int sign, Channel& channel, Message& message)
{
	BanInfo*								banInfo;
	Channel::Mode::rangePairMultimap_type	rangePair;
	Channel::Mode::multimap_iterator		maskIt;
	std::string								newMask;

	//TODO: verificar y completar mascara
	newMask = addWildcard(message[pos]);
	rangePair = channel.mode.getList(this->_chanModeConfig.mode);
	maskIt = findMask(rangePair, newMask);
	if (sign && maskIt == rangePair.second)
	{
		banInfo = new BanInfo(newMask, message.getSender().getName());
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

void	BanChanMode::onShowChanModeEvent(void)
{
}

void	BanChanMode::onDelete(void* pointer)
{
	delete &*reinterpret_cast<BanInfo* >(pointer);
}

std::string BanChanMode::toString(void* pointer)
{
	std::ostringstream	oss;
	BanInfo*			banInfo;

	banInfo	= reinterpret_cast<BanInfo* >(pointer);
	oss << banInfo->mask << " " << banInfo->nick << " :" << banInfo->time;
	return oss.str();
}
