#include "PartCommand.hpp"
#include "Channel.hpp"
#include "Numeric.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PartCommand::PartCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void PartCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	eventHandler.add(PARTEVENT, *new Delegate<PartCommand, Source>(*this, &PartCommand::partChannelEvent));
	eventHandler.add(NOTCHANEVENT, *new Delegate<PartCommand, Source>(*this, &PartCommand::notChannelEvent));
	eventHandler.add(NOTINCHANEVENT, *new Delegate<PartCommand, Source>(*this, &PartCommand::notInChannelEvent));
}

void PartCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void PartCommand::partChannelEvent(Source &source)
{
	Message &message = *source.message;

	message.setReceiver(message.getChannel()->getUserMap());
	message.setBroadcast(true);
	message.send();
}

void PartCommand::notChannelEvent(Source &source)
{
	Message &message = *source.message;

	message.setReceiver(message.getSender());
	Numeric::insertField(message[0]);
	message.send(Numeric::builder(message, ERR_NOSUCHCHANNEL));
}

void PartCommand::notInChannelEvent(Source &source)
{
	Message &message = *source.message;

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

