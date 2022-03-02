#include "UserCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"

#include <iostream>
#include <string>

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
	User*			user = this->userSender;

	if (user->getIdent() == "anonymous")
	{
		if (!this->server.getPass().empty() && user->getPass() != this->server.getPass())
		{
			message.limitMaxParam(1);
			message.setCmd("QUIT");
			message[0] = "Password incorret";
			message.process();
			return true;
		}
		user->setIdent(message[0]);
		if (atoi(message[1].c_str()) & 3)
			user->setMode(this->server.userModeFind('i'));
		user->setReal(message[3]);
		if (!user->getName().empty())
		{
			user->setStatus(LEVEL_NEGOTIATING);
			user->setPingChallenge("challenge-string");
			user->send("PING :" + user->getPingChallenge());
		}
	}
	else
		message.sendNumeric(ERR_ALREADYREGISTERED);
	return true;
}

bool	UserCommand::_recvServer(Message&)
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
