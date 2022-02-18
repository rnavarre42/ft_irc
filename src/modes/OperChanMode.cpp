#include "OperChanMode.hpp"
#include "User.hpp"
#include "Channel.hpp"
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

bool	OperChanMode::onChanModeEvent(int pos, int sign, Access &access, User &user, Channel &channel, Message &message)
{
	Server::userMap_iterator	targetIt;
	(void)access;
	(void)user;
	
	Console::log(LOG_DEBUG, "onChanModeEvent " + message[pos]);
	targetIt = channel.userFind(message[pos]);
	if (targetIt == channel.getUserMap().end())
	{
		Numeric::insertField(message[pos]);
		message.replyNumeric(ERR_NOSUCHNICK);
	}
	else	// Aquí se añade el modo a la lista del canal
	{
		if ((sign && this->setMode(channel, targetIt->second))
				|| (!sign && this->unsetMode(channel, targetIt->second)))
		{
			message[pos] = targetIt->second->getName();
			return true;
		}
	}
	return false;
}
/*
bool	OperChanMode::onDisableChanModeEvent(int pos, Access &access, User &user, Channel &channel, Message &message)
{
	Server::userMap_iterator	targetIt;
	(void)access;
	(void)user;
	targetIt = channel.userFind(message[pos]);
	if (targetIt == channel.getUserMap().end())
	{
		Numeric::insertField(message[pos]);
		message.replyNumeric(ERR_NOSUCHNICK);
	}
	else
	{
		if (this->unsetMode(channel, targetIt->second))
		{
			message[pos] = targetIt->second->getName();
		}
	}
	Console::log(LOG_DEBUG, "onDisableChanModeEvent " + message[pos]);
}
*/
void	OperChanMode::onShowChanModeEvent(void)
{
}
