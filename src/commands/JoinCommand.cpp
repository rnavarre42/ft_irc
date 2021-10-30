#include "JoinCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

JoinCommand::JoinCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool JoinCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
