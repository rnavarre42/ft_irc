#include "JoinCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"
#include "Console.hpp"
#include <iostream>

JoinCommand::JoinCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

JoinCommand::~JoinCommand(void)
{}

void	JoinCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	eventHandler.add(JOINEVENT,    *new Delegate<JoinCommand, Message>(*this, &JoinCommand::joinChannelEvent));
	eventHandler.add(NEWCHANEVENT, *new Delegate<JoinCommand, Message>(*this, &JoinCommand::createChannelEvent));
	eventHandler.add(ALREADYEVENT, *new Delegate<JoinCommand, Message>(*this, &JoinCommand::alreadyChannelEvent));
	eventHandler.add(ERRCHANEVENT, *new Delegate<JoinCommand, Message>(*this, &JoinCommand::errChannelEvent));
	eventHandler.add(MAXCHANEVENT, *new Delegate<JoinCommand, Message>(*this, &JoinCommand::limitChannelEvent));
	eventHandler.add(DELCHANEVENT, *new Delegate<JoinCommand, Message>(*this, &JoinCommand::delChannelEvent));
}

void	JoinCommand::unloadEvents(Server::eventHandler_type&)
{}

void	JoinCommand::createChannelEvent(Message& message)
{
	Console::log(LOG_INFO, message.getSender()->getName() + " ha creado el canal " + message[0]);
	message.setReceiver(message.getSender());
	message.hideReceiver();
	message.send();
	message.setCmd("NAMES");
	message.internal();
}

void	JoinCommand::joinChannelEvent(Message& message)
{
	Console::log(LOG_INFO, message.getSender()->getName() + " ha entrado al canal " + message[0]);
	message.setReceiver(message.getChannel());
	message.setReceiver(message.getSender());	// cuando el canal está vacio, hay que añadir a uno mismo
	message.hideReceiver();
	message.send();
	message.clearReceivers();
	message.setReceiver(message.getSender());
	message.setCmd("NAMES");
	message.internal();
}

void	JoinCommand::alreadyChannelEvent(Message& message)
{
	Console::log(LOG_INFO, message.getSender()->getName() + " ya está en " + message[0]);
}

void	JoinCommand::limitChannelEvent(Message& message)
{
	Console::log(LOG_INFO, message.getSender()->getName() + " ha alcanzado el limite de canales");
	message.setReceiver(message.getSender());
	Numeric::insertField(message[0]);
	message.send(Numeric::builder(message, ERR_TOOMANYCHANNELS));
}

void	JoinCommand::delChannelEvent(Message& message)
{
	Console::log(LOG_INFO, message.getChannel()->getName() + " el canal se ha eliminado");
}

void	JoinCommand::errChannelEvent(Message& message)
{
	message.setReceiver(message.getSender());
	Numeric::insertField(message[0]);
	message.send(Numeric::builder(message, ERR_BADCHANMASK));
}


bool	JoinCommand::_recvUser(Message& message)
{
	User*						user = this->senderUser;
	Server::channelMap_iterator	currentIt;

	if (message[0] == "0")
	{
		if (message.size() == 1)
			message.insertField("exit from all channels");
		message.setCmd("PART");
		for (Server::channelMap_iterator it = user->begin(); it != user->end();)
		{
			currentIt = it;
			++it;
			message[0] = currentIt->second->getName();
			this->server.delFromChannel(message);
			message.clearReceivers();
		}
	}
	else
		this->server.addToChannel(message);
	return true;
}

bool	JoinCommand::_recvServer(Message&)
{
	return false;
}

bool	JoinCommand::_recvUnknown(Message&)
{
	return false;
}

bool	JoinCommand::_sendUser(Message&)
{
	return false;
}

bool	JoinCommand::_sendServer(Message&)
{
	return false;
}
