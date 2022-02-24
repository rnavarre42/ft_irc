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
	this->_chanModeConfig.events = CHANMODE_TOPIC;
	this->_chanModeConfig.unique = true;
}

TopicChanMode::~TopicChanMode(void)
{}

void	TopicChanMode::onChanEvent(Access& access, int event, Message& message, int& numeric)
{
	Channel*	channel = message.getChannel();
	(void)event;
	if (!this->isSetMode(*channel))
		return ;
	if (message.getChannel()->isOper(message.getSender()))
		access = allow;
	else
	{
		Numeric::insertField(message.getChannel()->getName());
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

void	TopicChanMode::onDelete(void* )
{}

std::string TopicChanMode::toString(void* )
{
	return "";
}
