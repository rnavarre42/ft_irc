#include "KickCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <string>
#include <iostream>

KickCommand::KickCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void KickCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void KickCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

bool KickCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;
	std::string	data;

	for (int i = 0; i < 100; i++)
		data += "1u497yfhyuewoipvjfu4y2gr79ry23984u239fuhg23u48y239yguib23iur239g4f82739yr8923hury972yr892uer892hufu2";
	for (int i = 0; i < 10; i++)
		user.send(data);

	(void)message;
	(void)user;
	return false;
}

bool KickCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool KickCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool KickCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
