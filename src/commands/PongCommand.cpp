#include "PongCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PongCommand::PongCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void PongCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

bool PongCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	if (message[0] == user.getPingChallenge())
	{
		if (!user.isRegistered())
		{
			this->server.registerUser(message);
			//user.setRegistered(true);
			message.limitMaxParam(0);
			message.setReceiver(&user);
			message.setSender(this->server);
			message.setCmd("MOTD");
			this->server.sendCommand(message);
		}
		user.clearPingChallenge();
		user.setNextTimeout(0);
	}
	else if (!user.isRegistered())
	{
		message.limitMaxParam(1);
		message.setCmd("QUIT");
		message[0] = "Incorrect ping reply for registration";
		this->server.sendCommand(message);
//		this->server.delUser(user);
	}
//		server.killUser(user, "You should be dead now!");
	return true;
}

bool PongCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
bool PongCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool PongCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
