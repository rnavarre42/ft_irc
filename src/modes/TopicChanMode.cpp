#include "TopicChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Console.hpp"

TopicChanMode::TopicChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 't';
	this->_chanModeConfig.events = CHANMODE_TOPIC;
}

TopicChanMode::~TopicChanMode(void)
{}

void	TopicChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

bool	TopicChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &)
{
	(void)pos;

	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	TopicChanMode::onShowChanModeEvent(void)
{
}
