#include "PrivmsgCommand.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include "ChanModeConfig.hpp"

#include <iostream>

PrivmsgCommand::PrivmsgCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

PrivmsgCommand::~PrivmsgCommand(void)
{}

void	PrivmsgCommand::loadEvents(Server::eventHandler_type&)
{}

void	PrivmsgCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	PrivmsgCommand::_recvUser(Message& message)
{
	User*			user = this->senderUser;
	std::string&	target = message[0];
	Channel*		channel;
	User*		 	targetUser;

	if (message.size() < 2)
	{
		message.replyNumeric(ERR_NOTEXTTOSEND);
		return true;
	}
	if (this->server.isChannel(target))
	{
		if (!(channel = this->server.channelAt(target)))
		{	
			Numeric::insertField(target);
			message.replyNumeric(ERR_NOSUCHCHANNEL);
			return true;
		}
		message.setChannel(channel);
		if (!server.checkChannelMode(message, COMMAND_PRIVMSG))
			return true;
		message.setReceiver(channel);
		message.limitMaxParam(2);
		message.hideReceiver();
	}
	else
	{
		if (!(targetUser = this->server.userAt(target)))
		{
			Numeric::insertField(target);
			message.replyNumeric(ERR_NOSUCHNICK);
			return true;
		}
		message.eraseAt(0);
		message.setReceiver(targetUser);
		message.limitMaxParam(1);
	}
	message.setSender(user);
	message.send();
	return true;
}

bool	PrivmsgCommand::_recvServer(Message&)
{
	return false;
}

bool	PrivmsgCommand::_recvUnknown(Message&)
{
	return false;
}

bool	PrivmsgCommand::_sendUser(Message&)
{
	return false;
}

bool	PrivmsgCommand::_sendServer(Message&)
{
	return false;
}
