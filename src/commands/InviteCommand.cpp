#include "InviteCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

InviteCommand::InviteCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool InviteCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
