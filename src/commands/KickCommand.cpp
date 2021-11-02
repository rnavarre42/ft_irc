#include "KickCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <string>
#include <iostream>

KickCommand::KickCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool KickCommand::_execUser(Message &message)
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

bool KickCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
