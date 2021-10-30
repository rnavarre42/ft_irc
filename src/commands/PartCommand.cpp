#include "PartCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PartCommand::PartCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PartCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
