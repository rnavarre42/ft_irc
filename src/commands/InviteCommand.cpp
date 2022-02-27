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

void InviteCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void InviteCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

bool InviteCommand::_recvUser(Message& message)
{
	User*						user = this->userSender;
	Server::channelMap_iterator	channelIt;
	Server::userMap_iterator	invitedUserIt;
	Channel*					channel;
	User*						invitedUser;
	std::string					channelName, invitedName;

	message.setReceiver(message.getSender());
	if ((invitedUserIt = server.userFind(message[0])) == server.getUserMap().end())
	{
		Numeric::insertField(message[0]);
		message.sendNumeric(ERR_NOSUCHNICK);
		return true;
	}
	invitedUser = invitedUserIt->second;
	invitedName = invitedUser->getName();
	if ((channelIt = server.channelFind(message[1])) == server.getChannelMap().end())
	{
		Numeric::insertField(message[1]);
		message.sendNumeric(ERR_NOSUCHCHANNEL);
		return true;
	}
	channel = channelIt->second;
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
		Console::log(LOG_INFO, user->getName() + " ha invitado a " + invitedName + " a " + channelName);
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

bool	InviteCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool	InviteCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool	InviteCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
