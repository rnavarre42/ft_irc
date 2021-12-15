#include "VoiceChanMode.hpp"
#include "ChanModeConfig.hpp"

VoiceChanMode::VoiceChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::bothParam;
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

void	VoiceChanMode::onEnableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	VoiceChanMode::onDisableChanModeEvent(Access &access, std::string &value)
{
	(void)access;
	(void)value;
}

void	VoiceChanMode::onShowChanModeEvent(void)
{
}
