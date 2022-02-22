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
	Channel*	channel = message.getChannel();
	void*		limit = this->getMode(*channel);
//	size_t		temp = std::strtoul(this->getValue(limit).c_str(), NULL, 0);

	if (limit && channel->size() >= std::strtoul(this->getValue(limit).c_str(), NULL, 0))
	{
		Numeric::insertField(channel->getName());
		message.replyNumeric(ERR_CHANNELISFULL);
		access = AChanMode::deny;
	}
//	std::cout << "users: " << channel->size() << " : " << temp << std::endl;
}

inline bool	isValidLimit(unsigned long limit, Channel& channel, char chanMode)
{
	return !(limit == 0 || limit == ULONG_MAX
			|| limit == reinterpret_cast<unsigned long>(channel.mode[chanMode]));
}

bool	isNumber(std::string &str)
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
	unsigned long						limit;
	Channel::Mode::multimap_iterator	it;

	if (sign)
	{
		limit = std::strtoul(message[pos].c_str(), NULL, 0);
		it = channel.mode.find(this->_chanModeConfig.mode);
		if (isNumber(message[pos])
			&& isValidLimit(limit, channel, this->_chanModeConfig.mode))
		{
			if (it != channel.mode.end())
				it->second = reinterpret_cast<void *>(limit);
			else
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

void	LimitChanMode::onDelete(void *)
{
}

std::string LimitChanMode::getValue(void *pointer)
{
	std::ostringstream	oss;

	oss << reinterpret_cast<unsigned long>(pointer);
	return oss.str();
}
