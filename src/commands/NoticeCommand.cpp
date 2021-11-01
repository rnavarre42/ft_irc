#include "NoticeCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

NoticeCommand::NoticeCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool NoticeCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool NoticeCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
