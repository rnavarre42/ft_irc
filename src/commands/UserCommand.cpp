#include "UserCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"
#include <iostream>

UserCommand::UserCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void	UserCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void	UserCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

bool UserCommand::_recvUser(Message &message)
{
	User &user = *this->userSender;

	if (user.getIdent().empty())
	{
		user.setIdent(message[0]);
		user.setReal(message[3]);
		if (!user.getName().empty())
		{
			user.setPingChallenge("challenge-string");
			user.send("PING :" + user.getPingChallenge());
		}
	}
	else
		user.send(Numeric::builder(this->server, user, ERR_ALREADYREGISTERED));
	return true;
}

bool UserCommand::_recvServer(Message &message)
{
	Server &server = *this->serverSender;

	(void)server;
	(void)message;
	return false;
}

bool UserCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool UserCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
