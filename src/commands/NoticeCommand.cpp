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
	User&						user = *this->_senderUser;
	User*						targetUser;
	std::string					target;
	Channel*					targetChannel;

	if (message.size() < 2)
	{
		message.replyNumeric(ERR_NOTEXTTOSEND);
		return true;
	}
	target = message[0]; 
	if (this->_server.isChannel(target))
	{
		if (!(targetChannel = this->_server.channelAt(target)))
		{	
			Numeric::insertField(target);
			message.replyNumeric(ERR_NOSUCHCHANNEL);
			return true;
		}
		message.setReceiver(*targetChannel);
		message.limitMaxParam(2);
		message.hideReceiver();
	}
	else
	{
		if (!(targetUser = this->_server.userAt(target)))
		{
			Numeric::insertField(target);
			message.replyNumeric(ERR_NOSUCHNICK);
			return true;
		}
		message.eraseAt(0);
		message.setReceiver(*targetUser);
		message.limitMaxParam(1);
	}
	message.setSender(user);
	message.send();
	return true;
}

bool	NoticeCommand::_recvServer(Message&)
{
	return false;
}

bool	NoticeCommand::_recvUnknown(Message&)
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

bool	NoticeCommand::_sendUnknown(Message&)
{
	return false;
}
