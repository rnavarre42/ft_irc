#include "NoticeCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

NoticeCommand::NoticeCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool NoticeCommand::_exec(Message &message)
{
	(void)message;
	return false;
}
