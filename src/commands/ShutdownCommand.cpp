#include "ShutdownCommand.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Console.hpp"
#include <iostream>

ShutdownCommand::ShutdownCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

void ShutdownCommand::loadEvents(Server::eventHandler_type& )
{
//	eventHandler.add(QUITEVENT, *new Delegate<QuitCommand, Message>(*this, &QuitCommand::QuitEvent));
//	eventHandler.add(DELUSEREVENT, *new Delegate<QuitCommand, Message>(*this, &QuitCommand::DelUserEvent));
}

void ShutdownCommand::unloadEvents(Server::eventHandler_type& )
{
}

bool ShutdownCommand::_recvUser(Message& message)
{
	User&	user = *this->userSender;

	(void)user;
	server.quit("Server shutdown: reason (" + message[0] + ")");
	return true;
}

bool ShutdownCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool ShutdownCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)user;
	(void)message;
	return false;
}

bool ShutdownCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
