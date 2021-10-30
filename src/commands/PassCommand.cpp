#include "PassCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PassCommand::PassCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PassCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
