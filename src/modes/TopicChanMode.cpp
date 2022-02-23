#include "TopicChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "Console.hpp"

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
	(void)event;
	(void)numeric;
	if (message.getChannel()->isOper(message.getSender()))
		access = allow;
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
