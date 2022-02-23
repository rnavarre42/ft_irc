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
	this->_chanModeConfig.events = CHANMODE_JOIN;
	this->_chanModeConfig.unique = true;
}

LimitChanMode::~LimitChanMode(void)
{}

void	LimitChanMode::onChanEvent(Access& access, int event, Message& message)
{
	Channel*	channel = message.getChannel();
	size_t		limit = reinterpret_cast<size_t>(this->getMode(*channel));
	(void)event;

	if (limit && limit <= channel->size())
	{
		Numeric::insertField(channel->getName());
		message.replyNumeric(ERR_CHANNELISFULL);
		access = AChanMode::deny;
	}
	std::cout << "users: joined " << channel->size() << " : limit " << limit << std::endl;
}

inline bool	isValidLimit(unsigned long limit, Channel& channel, char chanMode)
{
	return !(limit == 0 || limit == ULONG_MAX
			|| limit == reinterpret_cast<unsigned long>(channel.mode[chanMode]));
}

bool	isNumber(std::string& str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (!std::isdigit(*it))
			return false;
	}
	return true;
}

bool	LimitChanMode::onChanModeEvent(int pos, int sign, Channel& channel, Message& message)
{
	unsigned long						limit;
//	Channel::Mode::multimap_iterator	modeIt;
	void*								value;

	if (sign)
	{
		limit = std::strtoul(message[pos].c_str(), NULL, 0);
	//	modeIt = channel.mode.find(this->_chanModeConfig.mode);
		if (isNumber(message[pos])
			&& isValidLimit(limit, channel, this->_chanModeConfig.mode))
		{
			value = reinterpret_cast<void*>(limit);
	//		if (modeIt != channel.mode.end())
	//			modeIt->second = value;
	//		else
			std::cout << "entra limit set mode" << std::endl;
			this->setMode(channel, value);
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
{}

void	LimitChanMode::onDelete(void* )
{}

std::string LimitChanMode::toString(void* pointer)
{
	std::ostringstream	oss;

	oss << reinterpret_cast<unsigned long>(pointer);
	return oss.str();
}
