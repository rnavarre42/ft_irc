#include "PassCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PassCommand::PassCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PassCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;

	user.setPass(*message.getParam(0));
	return true;
}

bool PassCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
