#include "ModeCommand.hpp"
#include "AChanMode.hpp"
#include "AUserMode.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Numeric.hpp"
#include "Console.hpp"
#include "ChanModeConfig.hpp"

#include <iostream>

ModeCommand::ModeCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

ModeCommand::~ModeCommand(void)
{}

void	ModeCommand::loadEvents(Server::eventHandler_type&)
{}

void	ModeCommand::unloadEvents(Server::eventHandler_type&)
{}

inline int	isSignMode(char c)
{
	return (c == '+') * 1 + (c == '-') * 2;
}

inline bool	hasParamMode(bool set, AChanMode* chanMode)
{
	return ((set && chanMode->getConfig().type & ChanModeConfig::enableParam)
			|| (!set && chanMode->getConfig().type & ChanModeConfig::disableParam));
}

void	cleanSignModes(std::string& modes)
{
	int	set = 3;

	for (std::string::iterator it = modes.begin(); it != modes.end();)
	{
		if ((isSignMode(*it) && ((1 + it) == modes.end() || isSignMode(*(1 + it))))
				|| isSignMode(*it) == set)
			modes.erase(it);
		else
		{
			if (isSignMode(*it))
				set = isSignMode(*it);
			++it;
		}
	}
}

/*
 *	Funcion que verifica si el modo de canal existe, la sintaxis es correcta y tiene privilegios para
 *	ejecutarlo.
 */

void	ModeCommand::_checkChanModes(Message& message)
{
	unsigned long					pos = 2;
	bool							set = true;
	std::string&					target = message[0];
	std::string&					modes = message[1];
	Channel*						channel;
	AChanMode*						chanMode;
	std::string::iterator 			charIt;
	size_t							initialSize;

	if (!(channel = server.channelAt(target)))
	{
		Numeric::insertField(target);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
		return ;
	}
	if (message.size() < 2)
	{
		Numeric::insertField(target);
		this->server.chanModeNames(*channel);
		message.replyNumeric(RPL_CHANNELMODEIS);
		Numeric::insertField(target);
		Numeric::insertField(to_string(channel->getCreationTime()));
		message.replyNumeric(RPL_CREATIONTIME);
		return ;
	}
	initialSize = modes.size();
	message.setChannel(channel);
	for (std::string::iterator stringIt = modes.begin(); stringIt != modes.end(); )
	{
		charIt = stringIt;
		++stringIt;
		if (*charIt == '+')
			set = true;
		else if (*charIt == '-')
			set = false;
		else if ((chanMode = server.chanModeFind(*charIt)))
		{
			if (!chanMode->getConfig().unique && initialSize == 1 && message.size() <= pos)
			{
				chanMode->onShowChanModeEvent(*channel, message);
				return ;
			}
			else if (hasParamMode(set, chanMode) && message.size() <= pos)
			{
				Numeric::insertField(message.getCmd());
				message.replyNumeric(ERR_NEEDMOREPARAMS);
				modes.erase(charIt);
				--stringIt;
			}
			else if (!server.checkChannelMode(message, COMMAND_MODE)) // Si el usuario no es operador...
			{
				modes.erase(charIt);
				--stringIt;
				if (hasParamMode(set, chanMode) && message.size() > pos)
					message.eraseAt(pos);
			}
			else
			{
				if (message.size() >= pos && chanMode->onChanModeEvent(pos, set, *channel, message))
				{
					if (hasParamMode(set, chanMode) && message.size() > pos)
						++pos;
				}
				else
				{
					modes.erase(charIt);
					--stringIt;
					if (hasParamMode(set, chanMode) && message.size() > pos)
						message.eraseAt(pos);
				}
			}
		}	
		else
		{
			Numeric::insertField(*charIt);
			Numeric::insertField(channel->getName());
			message.replyNumeric(ERR_UNKNOWNMODE);
			modes.erase(charIt);
			--stringIt;
		}
	}
	cleanSignModes(modes);
	message.limitMaxParam(pos);
	if (!modes.empty())
	{
		message.setReceiver(channel);
		message.setReceiver(this->userSender);
		message.hideReceiver();
		message.hideSemicolon();
		message.send();
	}
}

/*
 *	Funcion que verifica si el modo de usuario existe, la sintaxis es correcta y tiene privilegios para
 *	ejecutarlo.
 */ 

void	ModeCommand::_checkUserModes(Message& message)
{
	bool					set = true;
	std::string&			target = message[0];
	std::string&			modes = message[1];
	User*					user = static_cast<User*>(message.getSender());
	AUserMode*				userMode;
	std::string::iterator	charIt;

	if (!(user = server.userAt(target)))
	{
		Numeric::insertField(target);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
		return ;
	}
	if (*user != target)
	{
		Numeric::insertField(user->getName());
		message.replyNumeric(ERR_USERSDONTMATCH);
		return ;
	}
	if (message.size() < 2)
	{
		this->server.userModeNames(*user);
		message.sendNumeric(RPL_UMODEIS);
		return ;
	}
	for (std::string::iterator stringIt = modes.begin(); stringIt != modes.end(); )
	{
		charIt = stringIt;
		++stringIt;
		if (*charIt == '+')
			set = true;
		else if (*charIt == '-')
			set = false;
		else if ((userMode = server.userModeFind(*charIt)))
		{
			if (set && (userMode->getType() & AUserMode::canSet))
			{
				if (user->isSetMode(userMode))
				{
					modes.erase(charIt);
					--stringIt;
				}
				else
					user->setMode(userMode);
			}
			else if (!set && (userMode->getType() & AUserMode::canUnset))
			{
				if (user->isSetMode(userMode))
					user->unsetMode(userMode);
				else
				{
					modes.erase(charIt);
					--stringIt;
				}
			}
			else
			{
				modes.erase(charIt);
				--stringIt;
			}
		}
		else
		{
			Numeric::insertField(*charIt);
			Numeric::insertField(user->getName());
			message.replyNumeric(ERR_UNKNOWNMODE);
			modes.erase(charIt);
			--stringIt;
		}
	}
	cleanSignModes(modes);
	message.limitMaxParam(2);
	if (!modes.empty())
		message.reply();
}

bool	ModeCommand::_recvUser(Message& message)
{
	std::string&	channelName = message[0];

	if (server.isChannel((channelName)))
		this->_checkChanModes(message);
	else
		this->_checkUserModes(message);
	return true;
}

bool	ModeCommand::_recvServer(Message&)
{
	return false;
}

bool	ModeCommand::_sendUser(Message&)
{
	return false;
}

bool	ModeCommand::_sendServer(Message&)
{
	return false;
}
