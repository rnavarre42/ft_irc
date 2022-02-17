#include "ModerateChanMode.hpp"
#include "ChanModeConfig.hpp"

ModerateChanMode::ModerateChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 'm';
	this->_chanModeConfig.events = CHANMODE_PRIVMSG | CHANMODE_NOTICE;
}

ModerateChanMode::~ModerateChanMode(void)
{}

void	ModerateChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

void	ModerateChanMode::onEnableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message)
{
	(void)order;
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	ModerateChanMode::onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message)
{
	(void)order;
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	ModerateChanMode::onShowChanModeEvent(void)
{
}
