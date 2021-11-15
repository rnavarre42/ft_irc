#include "JoinCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"
#include "Console.hpp"
#include <iostream>

JoinCommand::JoinCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void JoinCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

bool JoinCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;
	Channel	*channel;

	channel = this->server.addToChannel(message);
	if (!channel)
	{
		Numeric::insertField(message[0]);
		user.send(Numeric::builder(this->server, user, ERR_BADCHANMASK));
	}
	return true;
}
/*
	if (flags & CHANNEL_JOIN)
	{
		if (flags & CHANNEL_CREATE)
			Console::log(LOG_INFO, "<" + user.getName() + "> ha creado " + channel->getName());
		else
			Console::log(LOG_INFO, "<" + user.getName() + "> ha entrado en " + channel->getName());
	}
	else if (flags & CHANNEL_CANTJOIN)
	{
		if (flags & CHANNEL_ISALREADY)
		{
			Numeric::insertField(user.getName());
			Numeric::insertField(channel->getName());
			user.send(Numeric::builder(this->server, user, ERR_USERONCHANNEL));
		}
		else
		{
			Numeric::insertField(channel->getName());
			if (flags & CHANNEL_ISFULL)
				user.send(Numeric::builder(this->server, user, ERR_CHANNELISFULL));
			else if (flags & CHANNEL_PRIVATE)
				user.send(Numeric::builder(this->server, user, ERR_INVITEONLYCHAN));
			else if (flags & CHANNEL_PASSWORD)
				user.send(Numeric::builder(this->server, user, ERR_BADCHANNELKEY));
			else if (flags & CHANNEL_BANNED)
				user.send(Numeric::builder(this->server, user, ERR_BANNEDFROMCHAN));
		}
	}
	return true;
}
*/
bool JoinCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool JoinCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool JoinCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
