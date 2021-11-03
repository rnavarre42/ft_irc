#include "JoinCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"
#include <iostream>

JoinCommand::JoinCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool JoinCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;

	if (message[0][0] == '#')
		;
	else
	{
		Numeric::insertField(message[0]);
		user.send(Numeric::builder(this->server, user, ERR_BADCHANMASK));
	}
	return true;
}

bool JoinCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
