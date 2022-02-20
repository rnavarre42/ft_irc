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

bool	VoiceChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &message)
{
	(void)pos;
	(void)sign;
	(void)message;
	(void)channel;
	return (false);
}
/*
void	VoiceChanMode::onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message)
{
	(void)order;
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}
*/
void	VoiceChanMode::onShowChanModeEvent(void)
{
}
