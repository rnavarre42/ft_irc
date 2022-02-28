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
	User*			user = this->userSender;
	Channel*		channel;
	User*			invitedUser;
	std::string		channelName, invitedName;

	message.setReceiver(message.getSender());
	if (!(invitedUser = server.userAt(message[0])))
	{
		Numeric::insertField(message[0]);
		message.sendNumeric(ERR_NOSUCHNICK);
		return true;
	}
	invitedName = invitedUser->getName();
	if (!(channel = server.channelAt(message[1])))
	{
		Numeric::insertField(message[1]);
		message.sendNumeric(ERR_NOSUCHCHANNEL);
		return true;
	}
	channelName = channel->getName();
	if (!user->isOnChannel(*channel))
	{
		Numeric::insertField(channelName);
		message.sendNumeric(ERR_NOTONCHANNEL);
		return true;
	}
	Numeric::insertField(invitedName);
	Numeric::insertField(channelName);
	if (invitedUser->isOnChannel(*channel))
		message.sendNumeric(ERR_USERONCHANNEL);
	else
	{
		server.invite().insert(invitedUser, channel);
		Console::log(LOG_INFO, "<" + user->getName() + "> has invited " + invitedName + " to <" + channelName + ">");
		message.sendNumeric(RPL_INVITING);
		message.clearReceiver();
		message.eraseAt(0);
		message[0] = channelName;
		message.setSender(user);
		message.setReceiver(invitedUser);
		message.send();
	}
	return true;
}

bool	InviteCommand::_recvServer(Message&)
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
