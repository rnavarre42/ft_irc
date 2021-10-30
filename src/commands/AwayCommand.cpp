#include "AwayCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

AwayCommand::AwayCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool AwayCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
