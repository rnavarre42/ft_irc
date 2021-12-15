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

void	LimitChanMode::onEnableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	LimitChanMode::onDisableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	LimitChanMode::onShowChanModeEvent(void)
{
}
