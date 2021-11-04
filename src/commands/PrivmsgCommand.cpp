#include "PrivmsgCommand.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PrivmsgCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;
	std::map<std::string, User *>::iterator	it;

	if (message.size() < 2)
		user.send(Numeric::builder(this->server, user, ERR_NOTEXTTOSEND));
	else if ((it = this->server.getUserMap().find(strToUpper(message[0]))) == this->server.getUserMap().end())
	{
		Numeric::insertField(message[0]);
		user.send(Numeric::builder(this->server, user, ERR_NOSUCHNICK));
	}
	else
	{
		message.eraseAt(0);
		message.setSender(user);
		message.setReceiver(it->second);
		message.limitMaxParam(1);
		it->second->send(message);
	}
	return true;
}

bool PrivmsgCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
bool PrivmsgCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool PrivmsgCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
