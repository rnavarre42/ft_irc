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

KickCommand::~KickCommand(void)
{}

void	KickCommand::loadEvents(Server::eventHandler_type&)
{}

void	KickCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	KickCommand::_recvUser(Message& message)
{
//	Server::channelMap_iterator	channelIt;
	Channel*					channel;
	User*						userKick;
//	Server::userMap_iterator	userKickIt;

	if (!isValidChanName(message[0]))
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_BADCHANMASK);
	}
	else if (!(channel = this->server.channelAt(message[0])))
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
	}
	else if (!(userKick = channel->at(message[1])))
	{
		Numeric::insertField(message[1]);
		message.replyNumeric(ERR_NOSUCHNICK);
	}
	else
	{
		std::cout << message.toString() << std::endl;
//		channel = channelIt->second;
		message.setChannel(*channel);
		if (!this->server.checkChannelMode(message, COMMAND_KICK))
			return true;
		message.setReceiver(*channel);
		message.setReceiver(message.getSender());
		message[0] = channel->getName();
		message[1] = userKick->getName();
		message.limitMaxParam(3);
		message.hideReceiver();
		std::cout << message.toString() << std::endl;
		message.send();
		server.removeUserFromChannel(*channel, *userKick);
	}
	return true;
}

bool	KickCommand::_recvServer(Message&)
{
	return false;
}

bool	KickCommand::_recvUnknown(Message&)
{
	return false;
}

bool	KickCommand::_sendUser(Message&)
{
	return false;
}

bool	KickCommand::_sendServer(Message&)
{
	return false;
}

bool	KickCommand::_sendUnknown(Message&)
{
	return false;
}
