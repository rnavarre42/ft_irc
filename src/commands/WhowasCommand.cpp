#include "WhowasCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

WhowasCommand::WhowasCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool WhowasCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
