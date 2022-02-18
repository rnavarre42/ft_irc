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

void	OperChanMode::onEnableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message)
{
	Server::userMap_iterator	targetIt;
	(void)order;
	(void)access;
	(void)user;
	if (message.size() == 2)
	{
		Numeric::insertField(message.getCmd());
		message.replyNumeric(ERR_NEEDMOREPARAMS);
	}
	else
	{
		targetIt = channel.userFind(message[2]);
		if (targetIt == channel.getUserMap().end())
		{
			Numeric::insertField(message[2]);
			message.replyNumeric(ERR_NOSUCHNICK);
		}
		else	// Aquí se añade el modo a la lista del canal
		{
			if (this->setMode(channel, targetIt->second))
			{
				message[2] = targetIt->second->getName();
				message.reply();
			}
		}
		Console::log(LOG_DEBUG, "onEnableChanModeEvent");
	}
}

void	OperChanMode::onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message)
{
	Server::userMap_iterator	targetIt;
	(void)order;
	(void)access;
	(void)user;
	if (message.size() == 2)
	{
		Numeric::insertField(message.getCmd());
		message.replyNumeric(ERR_NEEDMOREPARAMS);
	}
	else
	{
		targetIt = channel.userFind(message[2]);
		if (targetIt == channel.getUserMap().end())
		{
			Numeric::insertField(message[2]);
			message.replyNumeric(ERR_NOSUCHNICK);
		}
		else
		{
			if (this->unsetMode(channel, targetIt->second))
			{
				message[2] = targetIt->second->getName();
				message.reply();
			}
		}
		Console::log(LOG_DEBUG, "onDisableChanModeEvent");
	}
}

void	OperChanMode::onShowChanModeEvent(void)
{
}
