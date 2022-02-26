#include "OperCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include "User.hpp"

#include <iostream>

OperCommand::OperCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

void OperCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void OperCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

bool OperCommand::_recvUser(Message& message)
{
	User&						user = *this->userSender;

	(void)message;
	(void)user;

	if (message[0] != OPERLOGIN)
		message.replyNumeric(ERR_NOOPERHOST);
	else if (message[1] != OPERPASSWORD)
		message.replyNumeric(ERR_PASSWDMISMATCH);
	else
	{
		user.enableOper();
		message.replyNumeric(RPL_YOUREOPER);
	}
	return true;
}

bool OperCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool OperCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool OperCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
