#include "ModeCommand.hpp"
#include "AChanMode.hpp"
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
	Channel*						channel;
	AChanMode*						chanMode;
	std::string::iterator 			currentIt;

	if (message.size() < 2)
		//TODO Mostrar raw 324 (modes) 329 (creation time)
		return ;
	// Si el canal no existe...
	if (!(channel = server.channelAt(target)))
	{
		Numeric::insertField(target);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
		return ;
	}
	message.setChannel(*channel);
	for (std::string::iterator strIt = message[1].begin(); strIt != message[1].end(); )
	{
		currentIt = strIt;
		++strIt;
		if (*currentIt == '+')
			set = true;
		else if (*currentIt == '-')
			set = false;
		else if ((chanMode = server.findChanMode(*currentIt)))
		{
			if (hasParamMode(set, chanMode) && message.size() <= pos)
			{
				Numeric::insertField(message.getCmd());
				message.replyNumeric(ERR_NEEDMOREPARAMS);
				message[1].erase(currentIt);
				--strIt;
			}
			else if (!server.checkChannelMode(message, COMMAND_MODE)) // Si el usuario no es operador...
			{
				message[1].erase(currentIt);
				--strIt;
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
					message[1].erase(currentIt);
					--strIt;
					if (hasParamMode(set, chanMode) && message.size() > pos)
						message.eraseAt(pos);
				}
			}
		}	
		else
		{
			Numeric::insertField(*currentIt);
			Numeric::insertField(channel->getName());
			message.replyNumeric(ERR_UNKNOWNMODE);
			message[1].erase(currentIt);
			--strIt;
		}
	}
	cleanSignModes(message[1]);
	message.limitMaxParam(pos);
	if (!message[1].empty())
	{
		message.setReceiver(*channel);
		message.setReceiver(*this->senderUser);
		message.hideReceiver();
		message.send();
	}
}

/*
 *	Funcion que verifica si el modo de usuario existe, la sintaxis es correcta y tiene privilegios para
 *	ejecutarlo.
 */ 

void	ModeCommand::_checkUserModes(Message& message)
{
	Server::userMap_iterator	it = server.userFind(message[0]);

	if (it == this->server.getUserMap().end())
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_NOSUCHNICK);
		return ;
	}	  
	if (*this->senderUser != message[0])
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_USERSDONTMATCH);
		return ;
	}
}

bool	ModeCommand::_recvUser(Message& message)
{
	User&	user = *this->senderUser;

	(void)user;

	// El mensaje va destinado a un canal
	if (server.isChannel((message[0])))
		this->_checkChanModes(message);
	else // El mensaje va destinado al propio cliente.
		this->_checkUserModes(message);
	return true;
}

bool	ModeCommand::_recvServer(Message&)
{
	return false;
}

bool	ModeCommand::_recvUnknown(Message&)
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

bool	ModeCommand::_sendUnknown(Message&)
{
	return false;
}
