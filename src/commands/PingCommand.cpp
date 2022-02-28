#include "PingCommand.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Server.hpp"

#include <iostream>

PingCommand::PingCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

PingCommand::~PingCommand(void)
{}

void	PingCommand::loadEvents(Server::eventHandler_type&)
{}

void	PingCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	PingCommand::_recvUser(Message& message)
{
	User*		user = this->userSender;
	std::string	param;

	message.setReceiver(message.getSender());
	message.setSender(&this->server);
	message.setCmd("PONG");
	message.hideReceiver();
	if (message.size() == 1)
	{
		message.insertField(message[0]);
		message[0] = this->server.getName();
	}
	else
		message.swapField(0, 1);
	message.limitMaxParam(2);
	user->send(message);
	return true;
}

bool	PingCommand::_recvServer(Message&)
{
	return false;
}

bool	PingCommand::_sendUser(Message&)
{
	return false;
}

bool	PingCommand::_sendServer(Message&)
{
	return false;
}
