#include "WhoCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"

#include <iostream>

WhoCommand::WhoCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

WhoCommand::~WhoCommand(void)
{}

bool WhoCommand::_execUser(Message& message)
{
	User&	user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool WhoCommand::_execServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
