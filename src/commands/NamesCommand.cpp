#include "NamesCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

NamesCommand::NamesCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool NamesCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool NamesCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}