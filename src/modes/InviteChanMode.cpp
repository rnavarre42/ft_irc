#include "InviteChanMode.hpp"
#include "ChanModeConfig.hpp"

InviteChanMode::InviteChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 'i';
	this->_chanModeConfig.events = CHANMODE_JOIN;
}

InviteChanMode::~InviteChanMode(void)
{}

void	InviteChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

void	InviteChanMode::onEnableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	InviteChanMode::onDisableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	InviteChanMode::onShowChanModeEvent(void)
{
}
