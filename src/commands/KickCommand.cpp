#include "KickCommand.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include "chanmodes.hpp"

#include <string>
#include <iostream>

KickCommand::KickCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

void KickCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void KickCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

//KICK #canal nick :motivo
bool KickCommand::_recvUser(Message& message)
{
	User*						user = this->userSender;
	Server::channelMap_iterator	channelIt;
	Channel*					channel;

	(void)user;
	if ((channelIt = this->server.channelFind(message[0])) == this->server.getChannelMap().end())
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
	}
	else
	{
		channel = channelIt->second;
		message.setChannel(channel);
		if (!this->server.checkChannelMode(message, CHANMODE_KICK))
			return true;
	}
	
	return true;
}

bool KickCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool KickCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool KickCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
