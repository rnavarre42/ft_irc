#include "PingCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PingCommand::PingCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PingCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool PingCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}