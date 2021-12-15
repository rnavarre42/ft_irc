#include "OperChanMode.hpp"
#include "ChanModeConfig.hpp"

OperChanMode::OperChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::bothParam;
	this->_chanModeConfig.mode = 'o';
	this->_chanModeConfig.events = CHANMODE_KICK | CHANMODE_MODE | CHANMODE_INVITE;;
}

OperChanMode::~OperChanMode(void)
{}

void	OperChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

void	OperChanMode::onEnableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	OperChanMode::onDisableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	OperChanMode::onShowChanModeEvent(void)
{
}
