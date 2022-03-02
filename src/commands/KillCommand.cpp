#include "KillCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include "User.hpp"

#include <iostream>

KillCommand::KillCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

KillCommand::~KillCommand(void)
{}

void	KillCommand::loadEvents(Server::eventHandler_type&)
{}

void	KillCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	KillCommand::_recvUser(Message& message)
{
	User*						user = this->userSender;
	Server::userMap_iterator	userKillIt;
	std::string					userName;

	userName = message[0];
	if ((userKillIt = server.userFind(userName)) != server.getUserMap().end())
		server.deleteUser(*userKillIt->second, "Killed by " + user->getName() + ": " + message[1]);
	else
	{
		Numeric::insertField(userName);
		message.replyNumeric(ERR_NOSUCHNICK);
	}
	return true;
}

bool	KillCommand::_recvServer(Message&)
{
	return false;
}

bool	KillCommand::_sendUser(Message&)
{
	return false;
}

bool	KillCommand::_sendServer(Message&)
{
	return false;
}
