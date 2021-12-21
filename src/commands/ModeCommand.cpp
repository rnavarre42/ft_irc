#include "ModeCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Numeric.hpp"

#include <iostream>

ModeCommand::ModeCommand(Server &server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

void ModeCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void ModeCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

inline void ModeCommand::_checkChanModes(Message &message)
{
	Channel							*channel;
	Channel::channelMap_iterator	it = server.channelFind(message[0]);

	if (it == server.getChannelMap().end())
	{
		Numeric::insertField(message[0]);
		message.reply(Numeric::builder(message, ERR_NOSUCHCHANNEL));
		return ;
	}
	channel = it->second;
	if (!channel->isOper(this->userSender))
	{
		Numeric::insertField(channel->getName());
		message.reply(Numeric::builder(message, ERR_CHANOPRIVSNEEDED));
	}
}

inline static void _checkUserModes(Message &message)
{
	(void)message;
}

bool ModeCommand::_recvUser(Message &message)
{
	User						&user = *this->userSender;

	(void)user;

	if (message.getServer()->isChannel((message[0])))
		this->_checkChanModes(message);
	else
		_checkUserModes(message);
	return true;
}

bool ModeCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool ModeCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool ModeCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
