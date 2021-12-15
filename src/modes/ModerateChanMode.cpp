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

void	ModerateChanMode::onEnableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	ModerateChanMode::onDisableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	ModerateChanMode::onShowChanModeEvent(void)
{
}
