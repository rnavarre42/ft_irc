#include "PrivmsgCommand.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void PrivmsgCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void PrivmsgCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

bool PrivmsgCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;
	Server::userMap_iterator 	userIt;
	Server::channelMap_iterator	chanIt;

	if (message.size() < 2)
	{
		user.send(Numeric::builder(this->server, user, ERR_NOTEXTTOSEND));
		return true;
	}
	if (this->server.validChannelPrefix(message[0]))
	{
		if ((chanIt = this->server.findChannel(message[0])) == this->server.getChannelMap().end())
		{	
			Numeric::insertField(message[0]);
			user.send(Numeric::builder(this->server, user, ERR_NOSUCHCHANNEL));
			return true;
		}
		message.setReceiver(chanIt->second->getUserMap());
		message.limitMaxParam(2);
		message.setBroadcast(true);
	}
	else
	{
		if ((userIt = this->server.findUser(message[0])) == this->server.getUserMap().end())
		{
			Numeric::insertField(message[0]);
			user.send(Numeric::builder(this->server, user, ERR_NOSUCHNICK));
			return true;
		}
		message.eraseAt(0);
		message.setReceiver(userIt->second);
		message.limitMaxParam(1);
	}
	message.setSender(&user);
	message.send();
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
