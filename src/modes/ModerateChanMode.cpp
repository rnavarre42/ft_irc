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

bool	ModerateChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &)
{
	(void)pos;

	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	ModerateChanMode::onShowChanModeEvent(void)
{
}

void	ModerateChanMode::onDelete(void *)
{
}
