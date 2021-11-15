#include "PassCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PassCommand::PassCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void PassCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void PassCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

bool PassCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	user.setPass(message[0]);
	return true;
}

bool PassCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool PassCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool PassCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
