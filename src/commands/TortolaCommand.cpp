#include "PrivmsgCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

Command::Command(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool Command::_exec(Message &message)
{
	(void)message;
	return false;
}
