#include "WhowasCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"

#include <iostream>

WhowasCommand::WhowasCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

bool	WhowasCommand::_execUser(Message&)
{
	return false;
}

bool	WhowasCommand::_execServer(Message&)
{
	return false;
}
