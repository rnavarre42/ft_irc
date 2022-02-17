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

void	TopicChanMode::onEnableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message)
{
	(void)order;
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	TopicChanMode::onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message)
{
	(void)order;
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	TopicChanMode::onShowChanModeEvent(void)
{
}
