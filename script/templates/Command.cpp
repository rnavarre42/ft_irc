#include "${CLASS}Command.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

${CLASS}Command::${CLASS}Command(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool ${CLASS}Command::_execUser(Message &message)
{
	User	&user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool ${CLASS}Command::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
