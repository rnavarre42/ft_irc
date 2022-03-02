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
	Channel*					channel;
	User*						userKick;

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
		message.setChannel(channel);
		if (!this->server.checkChannelMode(message, COMMAND_KICK))
			return true;
		message.setReceiver(channel);
		message.setReceiver(message.getSender());
		message[0] = channel->getName();
		message[1] = userKick->getName();
		message.limitMaxParam(3);
		message.hideReceiver();
		message.send();
		server.removeUserFromChannel(*channel, *userKick);
	}
	return true;
}

bool	KickCommand::_recvServer(Message&)
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
