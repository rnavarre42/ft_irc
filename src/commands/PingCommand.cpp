#include "PingCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PingCommand::PingCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PingCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
