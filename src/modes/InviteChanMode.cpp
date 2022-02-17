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

void	InviteChanMode::onEnableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message)
{
	(void)order;
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	InviteChanMode::onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message)
{
	(void)order;
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
}

void	InviteChanMode::onShowChanModeEvent(void)
{
}
