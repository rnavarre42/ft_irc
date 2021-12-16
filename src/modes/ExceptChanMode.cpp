#include "ExceptChanMode.hpp"
#include "ChanModeConfig.hpp"

ExceptChanMode::ExceptChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::bothParam;
	this->_chanModeConfig.mode = 'e';
	this->_chanModeConfig.events = CHANMODE_JOIN | CHANMODE_PRIVMSG | CHANMODE_NOTICE | CHANMODE_NICK;
}

ExceptChanMode::~ExceptChanMode(void)
{}

void	ExceptChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

void	ExceptChanMode::onEnableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value)
{
	(void)access;
	(void)value;
	(void)user;
	(void)channel;
}

void	ExceptChanMode::onDisableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value)
{
	(void)access;
	(void)value;
	(void)user;
	(void)channel;
}

void	ExceptChanMode::onShowChanModeEvent(void)
{
}
