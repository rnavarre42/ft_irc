#include "WhoCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

WhoCommand::WhoCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool WhoCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
