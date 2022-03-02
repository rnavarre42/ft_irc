#include "LimitChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"

#include <cstdlib>
#include <climits>
#include <cctype>
#include <sstream>
#include <cstdlib>

LimitChanMode::LimitChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam;
	this->_chanModeConfig.mode = 'l';
	this->_chanModeConfig.events = COMMAND_JOIN;
	this->_chanModeConfig.unique = true;
}

LimitChanMode::~LimitChanMode(void)
{}

void	LimitChanMode::onChanEvent(Access& access, int, Message& message, int& numeric)
{
	Channel*	channel = message.getChannel();
	size_t		limit = reinterpret_cast<size_t>(this->getMode(*channel));

	if (limit && limit <= channel->size() && access != AChanMode::deny)
	{
		Numeric::insertField(channel->getName());
		numeric = ERR_CHANNELISFULL;
		access = AChanMode::deny;
	}
}

inline bool	isValidLimit(unsigned long limit, Channel& channel, char chanMode)
{
	return !(limit == 0 || limit == ULONG_MAX
			|| limit == reinterpret_cast<unsigned long>(channel.mode[chanMode]));
}

bool	isNumber(const std::string& str)
{
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if (!std::isdigit(*it))
			return false;
	}
	return true;
}

bool	LimitChanMode::onChanModeEvent(int pos, int sign, Channel& channel, Message& message)
{
	unsigned long	limit;

	if (sign)
	{
		limit = std::strtoul(message[pos].c_str(), NULL, 0);
		if (isNumber(message[pos])
			&& isValidLimit(limit, channel, this->_chanModeConfig.mode))
		{
			this->setMode(channel, reinterpret_cast<void*>(limit));
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

void	LimitChanMode::onShowChanModeEvent(Channel&, Message&)
{}

void	LimitChanMode::onDelete(void*)
{}

std::string LimitChanMode::toString(void* pointer)
{
	std::ostringstream	oss;

	oss << reinterpret_cast<unsigned long>(pointer);
	return oss.str();
}
