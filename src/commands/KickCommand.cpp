#include "KickCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

KickCommand::KickCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool KickCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
