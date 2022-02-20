#include "LimitChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include <cstdlib>
#include <climits>
#include <cctype>

LimitChanMode::LimitChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam;
	this->_chanModeConfig.mode = 'l';
	this->_chanModeConfig.events = CHANMODE_JOIN;
}

LimitChanMode::~LimitChanMode(void)
{}

void	LimitChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

inline bool	isValidLimit(unsigned long limit, Channel &channel, char chanMode)
{
	return !(limit == 0 || limit == ULONG_MAX || limit == reinterpret_cast<unsigned long>(channel.mode[chanMode]));
}

inline bool	isNumber(std::string &str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (!std::isdigit(*it))
			return false;
	}
	return true;
}

bool	LimitChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &message)
{
	unsigned long	limit;

	if (sign)
	{
		limit = std::strtoul(message[pos].c_str(), NULL, 0);
		if (isNumber(message[pos]) && isValidLimit(limit, channel, this->_chanModeConfig.mode))
		{
			this->setMode(channel, reinterpret_cast<void *>(limit));
			return true;
		}
	}
	else if (this->isSetMode(channel))
	{
		this->unsetMode(channel);
		return true;
	}
	return false;
}

void	LimitChanMode::onShowChanModeEvent(void)
{
}
