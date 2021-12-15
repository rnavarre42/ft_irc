#include "TopicChanMode.hpp"
#include "ChanModeConfig.hpp"

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

void	TopicChanMode::onEnableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	TopicChanMode::onDisableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	TopicChanMode::onShowChanModeEvent(void)
{
}
