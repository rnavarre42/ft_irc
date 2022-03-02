#include "KeyChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"

KeyChanMode::KeyChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'k';
	this->_chanModeConfig.events = COMMAND_JOIN;
	this->_chanModeConfig.unique = true;
}

KeyChanMode::~KeyChanMode(void)
{}

void	KeyChanMode::onChanEvent(Access& access, int, Message& message, int& numeric)
{
	Channel*	channel = message.getChannel();

	if (this->isSetMode(*channel))
	{
		if (message.size() == 1 || this->toString(this->getMode(*channel)) != message[1])
		{
			Numeric::insertField(channel->getName());
			numeric = ERR_BADCHANNELKEY;
			access = AChanMode::deny;
		}
	}
}

bool	KeyChanMode::onChanModeEvent(int pos, int sign, Channel& channel, Message& message)
{
	std::string*	password;

	if (sign)
	{
		if (this->isSetMode(channel))
		{
			if (*(password = static_cast<std::string*>(channel.mode[this->_chanModeConfig.mode])) == message[pos])
				return false;
			*password = message[pos];
		}
		else
			this->setMode(channel, new std::string(message[pos]));
	}
	else if (this->isSetMode(channel))
	{
		if (*(password = static_cast<std::string*>(channel.mode[this->_chanModeConfig.mode])) != message[pos])
			return false;
		else
			this->unsetMode(channel, password);
	}
	else
		return false;

	return true;
}

void	KeyChanMode::onShowChanModeEvent(Channel&, Message&)
{
}

void	KeyChanMode::onDelete(void* pointer)
{
	delete reinterpret_cast<std::string* >(pointer);
}

std::string KeyChanMode::toString(void* pointer)
{
	return *reinterpret_cast<std::string* >(pointer);
}
