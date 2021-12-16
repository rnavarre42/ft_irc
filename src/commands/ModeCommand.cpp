#include "ModeCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"

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

bool ModeCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	(void)message;
	(void)user;

	if (message.getServer()->isChannel((message[0])))
	{
		message.setReceiver(message.getSender());
		Numeric::insertField(message[0]);
		message.send(Numeric::builder(message, ERR_CHANOPRIVSNEEDED));
	}
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
