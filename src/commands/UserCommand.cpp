#include "UserCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Unknown.hpp"
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
	message.replyNumeric(ERR_ALREADYREGISTERED);
	return true;
}

bool	UserCommand::_recvServer(Message&)
{
	return false;
}

bool	UserCommand::_recvUnknown(Message& message)
{
	Unknown&	unknown = *this->_senderUnknown;;
	User*		newUser;

	if (unknown.getIdent() == "anonymous")
	{
		if (!this->_server.getPass().empty() && unknown.getPass() != this->_server.getPass())
		{
			message.limitMaxParam(1);
			message.setCmd("QUIT");
			message[0] = "Password incorret";
			message.internal();
			return true;
		}
		unknown.setIdent(message[0]);
		unknown.setReal(message[3]);
		if (!unknown.getName().empty())
		{
			unknown.setLevel(LEVEL_NEGOTIATING);
			unknown.setPingChallenge("challenge-string");
			unknown.send("PING :" + unknown.getPingChallenge());
		}
		newUser = new User(unknown);
	}
	return true;
}

bool	UserCommand::_sendUnknown(Message&)
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
