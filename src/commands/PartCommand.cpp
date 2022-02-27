#include "PartCommand.hpp"
#include "Console.hpp"
#include "Channel.hpp"
#include "Numeric.hpp"
#include "Message.hpp"
#include "Server.hpp"

#include <iostream>

PartCommand::PartCommand(Server &server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

PartCommand::~PartCommand(void)
{}

void	PartCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	eventHandler.add(PARTEVENT, *new Delegate<PartCommand, Message>(*this, &PartCommand::partChannelEvent));
	eventHandler.add(NOTCHANEVENT, *new Delegate<PartCommand, Message>(*this, &PartCommand::notChannelEvent));
	eventHandler.add(NOTINCHANEVENT, *new Delegate<PartCommand, Message>(*this, &PartCommand::notInChannelEvent));
}

void	PartCommand::unloadEvents(Server::eventHandler_type&)
{}

void	PartCommand::partChannelEvent(Message& message)
{
	Channel*	channel = message.getChannel();

	Console::log(LOG_INFO, message.getSender()->getName() + " ha salido de " + message.getChannel()->getName());
	message.setReceiver(channel);
	message.setReceiver(message.getSender());
	message.limitMaxParam(2);
	message.hideReceiver();
	message.send();
}

void	PartCommand::notChannelEvent(Message& message)
{
	message.setReceiver(message.getSender());
	Numeric::insertField(message[0]);
	message.send(Numeric::builder(message, ERR_NOSUCHCHANNEL));
}

void	PartCommand::notInChannelEvent(Message& message)
{
	message.setReceiver(message.getSender());
	Numeric::insertField(message[0]);
	message.send(Numeric::builder(message, ERR_NOTONCHANNEL));
}

bool	PartCommand::_recvUser(Message& message)
{
	this->server.delFromChannel(message);
	return true;
}

bool	PartCommand::_recvServer(Message&)
{
	return false;
}

bool	PartCommand::_sendUser(Message&)
{
	return false;
}

bool	PartCommand::_sendServer(Message&)
{
	return false;
}

