#include "UserCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"

#include <iostream>

UserCommand::UserCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

UserCommand::~UserCommand(void)
{}

void	UserCommand::loadEvents(Server::eventHandler_type&)
{}

void	UserCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	UserCommand::_recvUser(Message& message)
{
	User*	user = this->senderUser;

	if (user->getIdent() == "anonymous")
	{
		if (!this->server.getPass().empty() && user->getPass() != this->server.getPass())
		{
			message.limitMaxParam(1);
			message.setCmd("QUIT");
			message[0] = "Password incorret";
			message.internal();
			return true;
		}
		user->setIdent(message[0]);
		user->setReal(message[3]);
		if (!user->getName().empty())
		{
			user->setLevel(LEVEL_NEGOTIATING);
			user->setPingChallenge("challenge-string");
			user->send("PING :" + user->getPingChallenge());
		}
	}
	else
		message.replyNumeric(ERR_ALREADYREGISTERED);
	return true;
}

bool	UserCommand::_recvServer(Message&)
{
	return false;
}

bool	UserCommand::_recvUnknown(Message&)
{
	return false;
}

bool	UserCommand::_sendUser(Message&)
{
	return false;
}

bool	UserCommand::_sendServer(Message&)
{
	return false;
}
