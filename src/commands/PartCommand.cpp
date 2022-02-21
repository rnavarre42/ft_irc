#include "PartCommand.hpp"
#include "Console.hpp"
#include "Channel.hpp"
#include "Numeric.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PartCommand::PartCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void PartCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	eventHandler.add(PARTEVENT, *new Delegate<PartCommand, Message>(*this, &PartCommand::partChannelEvent));
	eventHandler.add(NOTCHANEVENT, *new Delegate<PartCommand, Message>(*this, &PartCommand::notChannelEvent));
	eventHandler.add(NOTINCHANEVENT, *new Delegate<PartCommand, Message>(*this, &PartCommand::notInChannelEvent));
}

void PartCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void PartCommand::partChannelEvent(Message &message)
{
	Console::log(LOG_INFO, message.getSender()->getName() + " ha salido de " + message.getChannel()->getName());
	message.setReceiver(message.getChannel());
	message.setReceiver(message.getSender());
	message.limitMaxParam(2);
	message.hideReceiver();
	message.send();
}

void PartCommand::notChannelEvent(Message &message)
{
	message.setReceiver(message.getSender());
	Numeric::insertField(message[0]);
	message.send(Numeric::builder(message, ERR_NOSUCHCHANNEL));
}

void PartCommand::notInChannelEvent(Message &message)
{
	message.setReceiver(message.getSender());
	Numeric::insertField(message[0]);
	message.send(Numeric::builder(message, ERR_NOTONCHANNEL));
}

bool PartCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	(void)user;
	this->server.delFromChannel(message);
	return true;
}

bool PartCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool PartCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;

	(void)message;
	(void)user;
	return false;
}

bool PartCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}

