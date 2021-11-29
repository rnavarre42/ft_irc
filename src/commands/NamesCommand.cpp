#include "NamesCommand.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include <iostream>

NamesCommand::NamesCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void NamesCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void NamesCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}
bool NamesCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	message.getServer()->names(*message.getChannel());
	message.setReceiver(message.getSender());
	message.send(Numeric::builder(message, RPL_NAMREPLY));
	(void)message;
	(void)user;
	return true;
}

bool NamesCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool NamesCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;

	(void)message;
	(void)user;
	return false;
}

bool NamesCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
