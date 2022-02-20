#include "KeyChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Channel.hpp"

KeyChanMode::KeyChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'k';
	this->_chanModeConfig.events = CHANMODE_JOIN;
}

KeyChanMode::~KeyChanMode(void)
{}

void	KeyChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

bool	KeyChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &message)
{
	std::string	*password;

	if (sign)
	{
		if (this->isSetMode(channel))
		{
			if (*(password = static_cast<std::string *>(channel.mode[this->_chanModeConfig.mode])) == message[pos])
				return false;
			*password = message[pos];
		}
		else
			this->setMode(channel, new std::string(message[pos]));
	}
	else if (this->isSetMode(channel))
	{
		if (*(password = static_cast<std::string *>(channel.mode[this->_chanModeConfig.mode])) != message[pos])
			return false;
		else
		{
			delete password;
			this->unsetMode(channel);
		}
	}
	else
		return false;

	return true;
}

void	KeyChanMode::onShowChanModeEvent(void)
{
}
