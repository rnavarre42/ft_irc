#include "PrivmsgCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PrivmsgCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool PrivmsgCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
