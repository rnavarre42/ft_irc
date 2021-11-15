#include "PartCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PartCommand::PartCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void PartCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void PartCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

bool PartCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	(void)message;
	(void)user;
	return false;
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

