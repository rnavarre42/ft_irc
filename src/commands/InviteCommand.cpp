#include "InviteCommand.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Console.hpp"
#include "Numeric.hpp"
#include <iostream>

InviteCommand::InviteCommand(Server &server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

void InviteCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void InviteCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

bool InviteCommand::_recvUser(Message &message)
{
	User						&user = *this->userSender;
	Server::channelMap_iterator	channelIt;
	Server::userMap_iterator	userIt;
	std::string					cName, uName;

	message.setReceiver(message.getSender());
	if ((userIt = server.userFind(message[0])) == server.getUserMap().end())
	{
		Numeric::insertField(message[0]);
		message.send(Numeric::builder(message, ERR_NOSUCHNICK));
		return true;
	}
	if ((channelIt = server.channelFind(message[1])) == server.getChannelMap().end())
	{
		Numeric::insertField(message[1]);
		message.send(Numeric::builder(message, ERR_NOSUCHCHANNEL));
		return true;
	}
	uName = userIt->second->getName();
	cName = channelIt->second->getName();
	Numeric::insertField(uName);
	Numeric::insertField(cName);
	if (userIt->second->isOnChannel(*channelIt->second))
		message.send(Numeric::builder(message, ERR_USERONCHANNEL));
	else
	{
		server.invite().insert(userIt->second, channelIt->second);
		Console::log(LOG_INFO, user.getName() + " ha invitado a " + uName + " a " + cName);
		message.send(Numeric::builder(message, RPL_INVITING));
		message.clearReceiver();
		message.eraseAt(0);
		message[0] = cName;
		message.setSender(&user);
		message.setReceiver(userIt->second);
		message.send();
	}
	return true;
}

bool InviteCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool InviteCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool InviteCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
