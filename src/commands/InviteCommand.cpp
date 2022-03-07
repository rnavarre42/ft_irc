#include "InviteCommand.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Console.hpp"
#include "Numeric.hpp"

#include <iostream>

InviteCommand::InviteCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

InviteCommand::~InviteCommand(void)
{}

void	InviteCommand::loadEvents(Server::eventHandler_type&)
{}

void	InviteCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	InviteCommand::_recvUser(Message& message)
{
	User&						user = *this->_senderUser;
	Channel*					channel;
	User*						invitedUser;
	std::string					channelName;
	std::string					invitedName;

	message.setReceiver(message.getSender());
	if (!(invitedUser = this->_server.userAt(message[0])))
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_NOSUCHNICK);
		return true;
	}
	invitedName = invitedUser->getName();
	if (!(channel = this->_server.channelAt(message[1])))
	{
		Numeric::insertField(message[1]);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
		return true;
	}
	channelName = channel->getName();
	if (!user.isOnChannel(*channel))
	{
		Numeric::insertField(channelName);
		message.replyNumeric(ERR_NOTONCHANNEL);
		return true;
	}
	Numeric::insertField(invitedName);
	Numeric::insertField(channelName);
	if (invitedUser->isOnChannel(*channel))
		message.replyNumeric(ERR_USERONCHANNEL);
	else
	{
		this->_server.invite().insert(*invitedUser, *channel);
		Console::log(LOG_INFO, user.getName() + " ha invitado a " + invitedName + " a " + channelName);
		message.replyNumeric(RPL_INVITING);
		message.clearReceivers();
		message.eraseAt(0);
		message[0] = channelName;
		message.setSender(user);
		message.setReceiver(*invitedUser);
		message.send();
	}
	return true;
}

bool	InviteCommand::_recvServer(Message&)
{
	return false;
}

bool	InviteCommand::_recvUnknown(Message&)
{
	return false;
}

bool	InviteCommand::_sendUser(Message&)
{
	return false;
}

bool	InviteCommand::_sendServer(Message&)
{
	return false;
}

bool	InviteCommand::_sendUnknown(Message&)
{
	return false;
}
