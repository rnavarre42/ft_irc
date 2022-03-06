#include "WhoCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"

#include <iostream>

WhoCommand::WhoCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

WhoCommand::~WhoCommand(void)
{}

bool	WhoCommand::_recvUser(Message& message)
{
	User&	user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool	WhoCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool	WhoCommand::_recvUnknown(Message&)
{
	return false;
}
