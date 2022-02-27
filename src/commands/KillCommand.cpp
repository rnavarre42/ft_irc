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

void KillCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void KillCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

bool KillCommand::_recvUser(Message& message)
{
	User&						user = *this->userSender;
	Server::userMap_iterator	userKillIt;

	(void)user;

	if ((userKillIt = server.userFind(message[0])) != server.getUserMap().end())
		server.deleteUser(*userKillIt->second, "Killed by " + user.getName() + ": " + message[1]);
	else
	{
		Numeric::insertField(userKillIt->second->getName());
		message.replyNumeric(ERR_NOSUCHNICK);
	}
	return true;
}

bool KillCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool KillCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool KillCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
