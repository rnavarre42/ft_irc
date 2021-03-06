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

void	NoticeCommand::loadEvents(Server::eventHandler_type&)
{}

void	NoticeCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	NoticeCommand::_recvUser(Message& message)
{
	User*						user = this->userSender;
	User*						targetUser;
	std::string					target;
	Channel*					targetChannel;

	if (message.size() < 2)
	{
		message.replyNumeric(ERR_NOTEXTTOSEND);
		return true;
	}
	target = message[0]; 
	if (this->server.isChannel(target))
	{
		if (!(targetChannel = this->server.channelAt(target)))
		{	
			Numeric::insertField(target);
			message.replyNumeric(ERR_NOSUCHCHANNEL);
			return true;
		}
		message.setReceiver(targetChannel);
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
	message.getSender()->setIdleTime(time(NULL));
	return true;
}

bool	NoticeCommand::_recvServer(Message&)
{
	return false;
}

bool	NoticeCommand::_sendUser(Message&)
{
	return false;
}

bool	NoticeCommand::_sendServer(Message&)
{
	return false;
}
