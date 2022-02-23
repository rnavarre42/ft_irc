#include "ListCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

ListCommand::ListCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

bool ListCommand::_recvUser(Message& message)
{
	User&	user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool ListCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool ListCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool ListCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	retur false;
}
