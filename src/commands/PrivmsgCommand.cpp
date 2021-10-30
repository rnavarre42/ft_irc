#include "PrivmsgCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PrivmsgCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
