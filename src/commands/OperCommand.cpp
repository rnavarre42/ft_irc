#include "OperCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include "User.hpp"

#include <iostream>

OperCommand::OperCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

OperCommand::~OperCommand(void)
{}

void	OperCommand::loadEvents(Server::eventHandler_type&)
{}

void	OperCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	OperCommand::_recvUser(Message& message)
{
	User*	user = this->senderUser;

	if (message[0] != OPERLOGIN)
		message.replyNumeric(ERR_NOOPERHOST);
	else if (message[1] != OPERPASSWORD)
		message.replyNumeric(ERR_PASSWDMISMATCH);
	else
	{
		user->enableOper();
		message.replyNumeric(RPL_YOUREOPER);
	}
	return true;
}

bool	OperCommand::_recvServer(Message&)
{
	return false;
}

bool	OperCommand::_sendUser(Message&)
{
	return false;
}

bool	OperCommand::_sendServer(Message&)
{
	return false;
}
