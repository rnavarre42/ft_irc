#include "PrivmsgCommand.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool PrivmsgCommand::_execUser(Message &message)
{
	User	&user = *this->userSender;

	if (message.size() < 2)
		user.send(Numeric::builder(this->server, user, ERR_NOTEXTTOSEND));
	if (this->server.getUserMap().find(strToUpper(*message.getParam(0))) == this->server.getUserMap().end())
		user.send(Numeric::builder(this->server, user, ERR_NOSUCHNICK, (std::string[]){*message.getParam(0)}, 1));
	(void)user;
	return true;
}

bool PrivmsgCommand::_execServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}