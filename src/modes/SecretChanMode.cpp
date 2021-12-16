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

void	SecretChanMode::onEnableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value)
{
	(void)access;
	(void)value;
	(void)user;
	(void)channel;
}

void	SecretChanMode::onDisableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value)
{
	(void)access;
	(void)value;
	(void)user;
	(void)channel;
}

void	SecretChanMode::onShowChanModeEvent(void)
{
}
