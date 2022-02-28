#include "ShutdownCommand.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Console.hpp"

#include <iostream>

ShutdownCommand::ShutdownCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

ShutdownCommand::~ShutdownCommand(void)
{}

void	ShutdownCommand::loadEvents(Server::eventHandler_type&)
{}

void	ShutdownCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	ShutdownCommand::_recvUser(Message& message)
{
	server.quit("Server shutdown: reason (" + message[0] + ")");
	return true;
}

bool	ShutdownCommand::_recvServer(Message&)
{
	return false;
}

bool	ShutdownCommand::_sendUser(Message&)
{
	return false;
}

bool	ShutdownCommand::_sendServer(Message&)
{
	return false;
}
