#include "NoExternalChanMode.hpp"
#include "ChanModeConfig.hpp"

NoExternalChanMode::NoExternalChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 'n';
	this->_chanModeConfig.events = CHANMODE_PRIVMSG | CHANMODE_PRIVMSG;
}

NoExternalChanMode::~NoExternalChanMode(void)
{}

void	NoExternalChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

void	NoExternalChanMode::onEnableChanModeEvent(Access &access, User &user, Channel &channel, Message &message)
{
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	NoExternalChanMode::onDisableChanModeEvent(Access &access, User &user, Channel &channel, Message &message)
{
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	NoExternalChanMode::onShowChanModeEvent(void)
{
}
