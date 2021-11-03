#include "PongCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PongCommand::PongCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PongCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;


	if (message[0] == user.getPingChallenge())
	{
		if (!user.isRegistered())
			user.setRegistered(true);
		user.clearPingChallenge();
		user.setNextTimeout(0);
	}
	else if (!user.isRegistered())
		user.send("You should be dead now!");
	return true;
}

bool PongCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
