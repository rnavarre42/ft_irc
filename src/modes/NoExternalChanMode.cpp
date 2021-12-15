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

void	NoExternalChanMode::onEnableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	NoExternalChanMode::onDisableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	NoExternalChanMode::onShowChanModeEvent(void)
{
}
