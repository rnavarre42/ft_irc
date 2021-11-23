#include "JoinCommand.hpp"
#include "Message.hpp"
#include "Source.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"
#include "Console.hpp"
#include <iostream>

JoinCommand::JoinCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void JoinCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	eventHandler.add(NEWCHANEVENT, *new Delegate<JoinCommand, Source>(*this, &JoinCommand::createChannelEvent));
	eventHandler.add(JOINEVENT, *new Delegate<JoinCommand, Source>(*this, &JoinCommand::joinChannelEvent));
	eventHandler.add(ALREADYEVENT, *new Delegate<JoinCommand, Source>(*this, &JoinCommand::alreadyChannelEvent));
	eventHandler.add(ERRCHANEVENT, *new Delegate<JoinCommand, Source>(*this, &JoinCommand::errChannelEvent));
	eventHandler.add(MAXCHANEVENT, *new Delegate<JoinCommand, Source>(*this, &JoinCommand::limitChannelEvent));
	eventHandler.add(DELCHANEVENT, *new Delegate<JoinCommand, Source>(*this, &JoinCommand::delChannelEvent));
}

void JoinCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void JoinCommand::createChannelEvent(Source &source)
{
	Message &message = *source.message;

	Console::log(LOG_INFO, message.getSender()->getName() + " ha creado el canal " + message[0]);
	message.setReceiver(message.getSender());
	message.setBroadcast(true);
	message.send();
}

void JoinCommand::joinChannelEvent(Source &source)
{
	Message &message = *source.message;
	Console::log(LOG_INFO, source.message->getSender()->getName() + " ha entrado al canal " + message[0]);
	message.setReceiver(message.getChannel()->getUserMap());
	message.setBroadcast(true);
	message.send();
	message.setReceiver(message.getSender());
	message.send();
//	message.setCmd("NAMES");
//	message.process();
}

void JoinCommand::alreadyChannelEvent(Source &source)
{
	Message &message = *source.message;

	Console::log(LOG_INFO, message.getSender()->getName() + " ya está en " + message[0]);
	(void)message;	
}

void JoinCommand::limitChannelEvent(Source &source)
{
	Message &message = *source.message;

	Console::log(LOG_INFO, message.getSender()->getName() + " ha alcanzado el limite de canales");
	message.setReceiver(message.getSender());
	Numeric::insertField(message[0]);
	message.send(Numeric::builder(source, ERR_TOOMANYCHANNELS));
}

void JoinCommand::delChannelEvent(Source &source)
{
	Message &message = *source.message;

	Console::log(LOG_INFO, message.getChannel()->getName() + " el canal se ha eliminado");
}

void JoinCommand::errChannelEvent(Source &source)
{
	Message &message = *source.message;

	message.setReceiver(message.getSender());
//	message.send();
	Numeric::insertField(message[0]);
	message.send(Numeric::builder(source, ERR_BADCHANMASK));
}


bool JoinCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	(void)user;
	this->server.addToChannel(message);
	return true;
}

bool JoinCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool JoinCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;

	(void)message;
	(void)user;
	return false;
}

bool JoinCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
