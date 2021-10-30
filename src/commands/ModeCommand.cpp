#include "ModeCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

ModeCommand::ModeCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool ModeCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
