#include "SecretChanMode.hpp"
#include "ChanModeConfig.hpp"

SecretChanMode::SecretChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 's';
	this->_chanModeConfig.events = 0;
	this->_chanModeConfig.unique = true;
}

SecretChanMode::~SecretChanMode(void)
{}

void	SecretChanMode::onChanEvent(Access&, int, Message&, int&)
{}

bool	SecretChanMode::onChanModeEvent(int, int sign, Channel& channel, Message&)
{
	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	SecretChanMode::onShowChanModeEvent(Channel&, Message&)
{}

void	SecretChanMode::onDelete(void*)
{}

std::string	SecretChanMode::toString(void*)
{
	return "";
}
