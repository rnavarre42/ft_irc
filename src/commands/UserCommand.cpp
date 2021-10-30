#include "UserCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

UserCommand::UserCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool UserCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
