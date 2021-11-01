#include "WhowasCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

WhowasCommand::WhowasCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool WhowasCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool WhowasCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
