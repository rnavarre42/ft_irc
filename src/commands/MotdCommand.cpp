#include "MotdCommand.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"

#include <iostream>
#include <fstream>

MotdCommand::MotdCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

MotdCommand::~MotdCommand(void)
{}

void	MotdCommand::loadEvents(Server::eventHandler_type&)
{}

void	MotdCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	MotdCommand::_recvUser(Message&)
{
	return true;
}

bool	MotdCommand::_recvServer(Message&)
{
	return false;
}

bool	MotdCommand::_sendUser(Message& message)
{
	User*			user = this->receiverUser;
	std::ifstream	ifs;
	std::string		line;

	ifs.open("motd.txt", std::fstream::in);
	if (ifs.fail())
	{
		message.send(Numeric::builder(this->server, *user, ERR_NOMOTD));
		return true;
	}
	Numeric::insertField(this->server.getMask());
	user->sendToBuffer(Numeric::builder(this->server, *user, RPL_MOTDSTART));
	while (std::getline(ifs, line))
	{
		Numeric::insertField(line);
		user->sendToBuffer(Numeric::builder(this->server, *user, RPL_MOTD));
	}
	user->send(Numeric::builder(this->server, *user, RPL_ENDOFMOTD));
	ifs.close();
	return true;
}

bool	MotdCommand::_recvUnknown(Message&)
{
	return false;
}

bool	MotdCommand::_sendServer(Message&)
{
	return false;
}

bool	MotdCommand::_sendUnknown(Message&)
{
	return false;
}
