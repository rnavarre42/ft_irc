#include "${CLASS}Command.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

${CLASS}Command::${CLASS}Command(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool ${CLASS}Command::_exec(Message &message)
{
	(void)message;
	return false;
}
