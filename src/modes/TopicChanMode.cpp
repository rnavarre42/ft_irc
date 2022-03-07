#include "TopicChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "Console.hpp"
#include "Numeric.hpp"

TopicChanMode::TopicChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 't';
	this->_chanModeConfig.events = COMMAND_TOPIC;
	this->_chanModeConfig.unique = true;
}

TopicChanMode::~TopicChanMode(void)
{}

void	TopicChanMode::onChanEvent(Access& access, int, Message& message, int& numeric)
{
	Channel&	channel = message.getChannel();

	if (!this->isSetMode(channel))
		return ;
	if (message.getChannel().isOper(message.getSender()))
		access = allow;
	else
	{
		Numeric::insertField(channel.getName());
		numeric = ERR_CHANOPRIVSNEEDED;
		access = AChanMode::deny;
	}
}

bool	TopicChanMode::onChanModeEvent(int, int sign, Channel& channel, Message& )
{
	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	TopicChanMode::onShowChanModeEvent(void)
{}

void	TopicChanMode::onDelete(void*)
{}

std::string TopicChanMode::toString(void*)
{
	return "";
}
