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

bool	InviteChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &)
{
	(void)pos;

	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	InviteChanMode::onShowChanModeEvent(void)
{
}

void	InviteChanMode::onDelete(void *)
{
}
