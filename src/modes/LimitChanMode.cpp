#include "LimitChanMode.hpp"
#include "ChanModeConfig.hpp"

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
	(void)access;
	(void)message;
}

void	LimitChanMode::onEnableChanModeEvent(Access &access, User &user, Channel &channel, Message &message)
{
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	LimitChanMode::onDisableChanModeEvent(Access &access, User &user, Channel &channel, Message &message)
{
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	LimitChanMode::onShowChanModeEvent(void)
{
}
