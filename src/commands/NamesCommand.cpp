#include "NamesCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

NamesCommand::NamesCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool NamesCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
