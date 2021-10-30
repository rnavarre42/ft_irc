#include "NickCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

NickCommand::NickCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool NickCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
