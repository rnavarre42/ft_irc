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
	User*						user = this->userSender;
	Server::userMap_iterator 	userIt;
	Server::channelMap_iterator	chanIt;

	if (message.size() < 2)
	{
		message.replyNumeric(ERR_NOTEXTTOSEND);
		return true;
	}
	if (this->server.isChannel(message[0]))
	{
		if ((chanIt = this->server.channelFind(message[0])) == this->server.getChannelMap().end())
		{	
			Numeric::insertField(message[0]);
			message.replyNumeric(ERR_NOSUCHCHANNEL);
			return true;
		}
		message.setChannel(chanIt->second);
		if (!server.checkChannelMode(message, COMMAND_PRIVMSG))
			return true;
		message.setReceiver(chanIt->second);
		message.limitMaxParam(2);
		message.hideReceiver();
	}
	else
	{
		if ((userIt = this->server.userFind(message[0])) == this->server.getUserMap().end())
		{
			Numeric::insertField(message[0]);
			message.replyNumeric(ERR_NOSUCHNICK);
			return true;
		}
		message.eraseAt(0);
		message.setReceiver(userIt->second);
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
bool	PrivmsgCommand::_sendUser(Message&)
{
	return false;
}

bool	PrivmsgCommand::_sendServer(Message&)
{
	return false;
}
