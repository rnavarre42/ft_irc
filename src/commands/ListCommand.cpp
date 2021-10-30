#include "ListCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

ListCommand::ListCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool ListCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
