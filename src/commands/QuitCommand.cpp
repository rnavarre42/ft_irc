#include "QuitCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

QuitCommand::QuitCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool QuitCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;

	if (!message.size())
		server.killUser(user, "");
	else
		server.killUser(user, message[0]);
	return true;
}

bool QuitCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
/*
void	QuitCommand::_send(Message &message)
{
	std::string	reason;

	if (!message.size())
		reason = "Client exited";
	else
		reason = "Quit: " + message[0];
}
*/
