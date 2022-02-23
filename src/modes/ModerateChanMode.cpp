#include "ModerateChanMode.hpp"
#include "ChanModeConfig.hpp"

ModerateChanMode::ModerateChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 'm';
	this->_chanModeConfig.events = CHANMODE_PRIVMSG | CHANMODE_NOTICE;
	this->_chanModeConfig.unique = true;
}

ModerateChanMode::~ModerateChanMode(void)
{}

void	ModerateChanMode::onChanEvent(Access& access, int event, Message& message, int& numeric)
{
	(void)access;
	(void)event;
	(void)message;
	(void)numeric;
}

bool	ModerateChanMode::onChanModeEvent(int, int sign, Channel& channel, Message& )
{
	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	ModerateChanMode::onShowChanModeEvent(void)
{
}

void	ModerateChanMode::onDelete(void* )
{
}

std::string ModerateChanMode::toString(void* )
{
	return "";
}
