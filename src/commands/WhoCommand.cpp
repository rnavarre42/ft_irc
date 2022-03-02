#include "WhoCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"

#include <iostream>

WhoCommand::WhoCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

WhoCommand::~WhoCommand(void)
{}

bool	WhoCommand::_execUser(Message&)
{
	return false;
}

bool	WhoCommand::_execServer(Message&)
{
	return false;
}
