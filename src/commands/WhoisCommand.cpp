#include "WhoisCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

WhoisCommand::WhoisCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

bool WhoisCommand::_execUser(Message& message)
{
	User&	user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool WhoisCommand::_execServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
