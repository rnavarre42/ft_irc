#include "PongCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PongCommand::PongCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PongCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
