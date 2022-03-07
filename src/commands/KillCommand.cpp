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
	User*						user = this->_senderUser;
	Server::userMap_iterator	userKillIt;

	if ((userKillIt = this->_server.userFind(message[0])) != this->_server.getUserMap().end())
		this->_server.deleteUser(*userKillIt->second, "Killed by " + user->getName() + ": " + message[1]);
	else
	{
		Numeric::insertField(userKillIt->second->getName());
		message.replyNumeric(ERR_NOSUCHNICK);
	}
	return true;
}

bool	KillCommand::_recvServer(Message&)
{
	return false;
}

bool	KillCommand::_recvUnknown(Message&)
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

bool	KillCommand::_sendUnknown(Message&)
{
	return false;
}
