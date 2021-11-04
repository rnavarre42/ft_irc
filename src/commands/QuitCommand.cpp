#include "QuitCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

QuitCommand::QuitCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool QuitCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	if (!message.size())
		server.killUser(user, "");
	else
		server.killUser(user, message[0]);
	return true;
}

bool QuitCommand::_recvServer(Message &message)
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

bool QuitCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool QuitCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
