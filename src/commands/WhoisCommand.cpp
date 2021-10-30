#include "WhoisCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

WhoisCommand::WhoisCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool WhoisCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
