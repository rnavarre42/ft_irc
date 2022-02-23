#include "NoExternalChanMode.hpp"
#include "ChanModeConfig.hpp"

NoExternalChanMode::NoExternalChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 'n';
	this->_chanModeConfig.events = CHANMODE_PRIVMSG | CHANMODE_PRIVMSG;
}

NoExternalChanMode::~NoExternalChanMode(void)
{}

void	NoExternalChanMode::onChanEvent(Access &access, int event, Message &message)
{
	(void)access;
	(void)event;
	(void)message;
}

bool	NoExternalChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &)
{
	(void)pos;

	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	NoExternalChanMode::onShowChanModeEvent(void)
{
}

void	NoExternalChanMode::onDelete(void *)
{
}

std::string NoExternalChanMode::toString(void *)
{
	return "";
}
