#include "MotdCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include <iostream>
#include <fstream>

MotdCommand::MotdCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool MotdCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	message.setReceiver(&user);
	this->server.sendCommand(message);

	(void)user;
	return true;
}

bool MotdCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool MotdCommand::_sendUser(Message &message)
{
	User			&user = *this->userReceiver;
	std::ifstream	ifs;
	std::string		line;

	ifs.open("motd.txt", std::fstream::in);
	if (ifs.fail())
	{
		message.getSender().send(Numeric::builder(this->server, user, ERR_NOMOTD));
		return true;
	}
	Numeric::insertField(this->server.getMask());
	user.send(Numeric::builder(this->server, user, RPL_MOTDSTART));
	while (std::getline(ifs, line))
	{
		Numeric::insertField(line);
		user.send(Numeric::builder(this->server, user, RPL_MOTD));
	}
	user.send(Numeric::builder(this->server, user, RPL_ENDOFMOTD));
	ifs.close();
	return true;
}

bool MotdCommand::_sendServer(Message &message)
{
	Server &server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
