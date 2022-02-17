#include "OperChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Console.hpp"

OperChanMode::OperChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'o';
	this->_chanModeConfig.events = CHANMODE_KICK | CHANMODE_MODE | CHANMODE_INVITE;;
}

OperChanMode::~OperChanMode(void)
{}

void	OperChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
}

void	OperChanMode::onEnableChanModeEvent(Access &access, User &user, Channel &channel, Message &message)
{
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
	Console::log(LOG_DEBUG, "onEnableChanModeEvent");
}

void	OperChanMode::onDisableChanModeEvent(Access &access, User &user, Channel &channel, Message &message)
{
	(void)access;
	(void)message;
	(void)user;
	(void)channel;
	Console::log(LOG_DEBUG, "onDisableChanModeEvent");
}

void	OperChanMode::onShowChanModeEvent(void)
{
}
