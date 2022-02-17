#include "ModeCommand.hpp"
#include "AChanMode.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Numeric.hpp"
#include "Console.hpp"

#include <iostream>

ModeCommand::ModeCommand(Server &server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

void ModeCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void ModeCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

/*
 *	Funcion que verifica si el modo de canal existe, la sintaxis es correcta y tiene privilegios para
 *	ejecutarlo.
 */
inline void ModeCommand::_checkChanModes(Message &message)
{
	int								order = 0;
	bool							set = true;
	Channel							*channel;
	Channel::channelMap_iterator	it = server.channelFind(message[0]);
	AChanMode						*chanMode;
	AChanMode::Access				access;

	if (message.size() < 2)
		//TODO Mostrar raw 324 (modes) 329 (creation time)
		return ;
	// Si el canal no existe...
	if (it == server.getChannelMap().end())
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
		return ;
	}
	channel = it->second;
	// Si el usuario no es operador...
	if (channel->isOper(this->userSender))
	{
		Numeric::insertField(channel->getName());
		message.replyNumeric(ERR_CHANOPRIVSNEEDED);
		return ;
	}
	for (std::string::iterator strIt = message[1].begin(); strIt != message[1].end(); ++strIt)
	{
		if (*strIt == '+')
			set = true;
		else if (*strIt == '-')
			set = false;
		else if ((chanMode = server.findChanMode(*strIt)))
		{
			if (set)
			{
				chanMode->onEnableChanModeEvent(order, access, *this->userSender, *channel, message);
				if (chanMode->getConfig().type & ChanModeConfig::enableParam && message.size() > 2)
					message.eraseAt(2);
			}
			else
			{
				chanMode->onDisableChanModeEvent(order, access, *this->userSender, *channel, message);
				if (chanMode->getConfig().type & ChanModeConfig::disableParam && message.size() > 2)
					message.eraseAt(2);
			}
			++order;
		}
		else
		{
			Numeric::insertField(*strIt);
			Numeric::insertField(channel->getName());
			message.replyNumeric(ERR_UNKNOWNMODE);
		}
	}
}

/*
 *	Funcion que verifica si el modo de usuario existe, la sintaxis es correcta y tiene privilegios para
 *	ejecutarlo.
 */ 

inline void ModeCommand::_checkUserModes(Message &message)
{
	Server::userMap_iterator	it = server.userFind(message[0]);

	if (it == this->server.getUserMap().end())
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_NOSUCHNICK);
		return ;
	}	  
	if (*this->userSender != message[0])
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_USERSDONTMATCH);
		return ;
	}
}

bool ModeCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	(void)user;

	// El mensaje va destinado a un canal
	if (server.isChannel((message[0])))
		this->_checkChanModes(message);
	else // El mensaje va destinado al propio cliente.
		this->_checkUserModes(message);
	return true;
}

bool ModeCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool ModeCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool ModeCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
