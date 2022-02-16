#include "VoiceChanMode.hpp"
#include "ChanModeConfig.hpp"

VoiceChanMode::VoiceChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'v';
	this->_chanModeConfig.events = 0;
}

VoiceChanMode::~VoiceChanMode(void)
{}

void	VoiceChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

void	VoiceChanMode::onEnableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value)
{
	(void)access;
	(void)value;
	(void)user;
	(void)channel;
}

void	VoiceChanMode::onDisableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value)
{
	(void)access;
	(void)value;
	(void)user;
	(void)channel;
}

void	VoiceChanMode::onShowChanModeEvent(void)
{
}
