#include "KickCommand.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include "ChanModeConfig.hpp"

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
// kick #canal nick :motivo
bool KickCommand::_recvUser(Message& message)
{
//	User*						user = this->userSender;
	Server::channelMap_iterator	channelIt;
	Channel*					channel;
	Server::userMap_iterator	userKickIt;

	if (!isValidChanName(message[0]))
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_BADCHANMASK);
	}
	else if ((channelIt = this->server.channelFind(message[0])) == this->server.getChannelMap().end())
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
	}
	else if ((userKickIt = channelIt->second->find(message[1])) == channelIt->second->end())
	{
		Numeric::insertField(message[1]);
		message.replyNumeric(ERR_NOSUCHNICK);
	}
	else
	{
		std::cout << message.toString() << std::endl;
		channel = channelIt->second;
		message.setChannel(channel);
		if (!this->server.checkChannelMode(message, COMMAND_KICK))
			return true;
		message.setReceiver(channel);
		message.setReceiver(message.getSender());
		message[0] = channel->getName();
		message[1] = userKickIt->second->getName();
		message.limitMaxParam(3);
		message.hideReceiver();
		std::cout << message.toString() << std::endl;
		message.send();
		server.removeUserFromChannel(*channel, *userKickIt->second);
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
