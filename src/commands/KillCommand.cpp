#include "KillCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

KillCommand::KillCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

void KillCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void KillCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

bool KillCommand::_recvUser(Message& message)
{
	User&	user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool KillCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool KillCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool KillCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
