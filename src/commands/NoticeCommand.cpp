#include "NoticeCommand.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Numeric.hpp"

#include <iostream>

NoticeCommand::NoticeCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

NoticeCommand::~NoticeCommand(void)
{}

void	NoticeCommand::loadEvents(Server::eventHandler_type& )
{}

void	NoticeCommand::unloadEvents(Server::eventHandler_type& )
{}

bool	NoticeCommand::_recvUser(Message& message)
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

bool	NoticeCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool	NoticeCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;

	(void)user;
	(void)message;
	return false;
}

bool	NoticeCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)server;
	(void)message;
	return false;
}
