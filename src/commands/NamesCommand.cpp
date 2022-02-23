#include "NamesCommand.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include <iostream>

NamesCommand::NamesCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

void NamesCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void NamesCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}
bool NamesCommand::_recvUser(Message& message)
{
	Server::channelMap_iterator	it;
	User&	user = *this->userSender;

	(void)user;
	message.setReceiver(message.getSender());
	if ((it = message.getServer()->channelFind(message[0])) != message.getServer()->getChannelMap().end())
	{
		message.setChannel(it->second);
		message.getServer()->names(*message.getChannel());
		message.sendNumeric(RPL_NAMREPLY);
	}
	Numeric::insertField(message[0]);
	message.sendNumeric(RPL_ENDOFNAMES);
	return true;
}

bool NamesCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool NamesCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;

	(void)message;
	(void)user;
	this->_recvUser(message);
	return false;
}

bool NamesCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
