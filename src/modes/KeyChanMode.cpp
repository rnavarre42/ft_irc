#include "KeyChanMode.hpp"
#include "ChanModeConfig.hpp"

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

void	KeyChanMode::onEnableChanModeEvent(Access &access, User &user, Channel &channel, Message &message)
{
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	KeyChanMode::onDisableChanModeEvent(Access &access, User &user, Channel &channel, Message &message)
{
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	KeyChanMode::onShowChanModeEvent(void)
{
}
