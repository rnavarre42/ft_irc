#include "SecretChanMode.hpp"
#include "ChanModeConfig.hpp"

SecretChanMode::SecretChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 's';
	this->_chanModeConfig.events = 0;
}

SecretChanMode::~SecretChanMode(void)
{}

void	SecretChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

bool	SecretChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &)
{
	(void)pos;

	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	SecretChanMode::onShowChanModeEvent(void)
{
}

void	SecretChanMode::onDelete(void *)
{
}
