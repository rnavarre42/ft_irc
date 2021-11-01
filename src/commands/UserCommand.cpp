#include "UserCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"
#include <iostream>

UserCommand::UserCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool UserCommand::_execUser(Message &message)
{
	User &user = *this->userSender;

	if (user.getIdent().empty())
	{
		user.setIdent(*message.getParam(0));
		user.setReal(*message.getParam(3));
		if (!user.getName().empty())
			user.setRegistered(true);
	}
	else
		user.send(Numeric::builder(this->server, message, ERR_ALREADYREGISTERED));
	return true;
}

bool UserCommand::_execServer(Message &message)
{
	Server &server = *this->serverSender;

	(void)server;
	(void)message;
	return false;
}
