#include "ACommand.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Unknown.hpp"
#include "User.hpp"

#include <string>
#include <iostream>

ACommand::ACommand(Server& server, int levelAccess, int minParam)
	: server(server)
	, levelAccess(levelAccess)
	, minParam(minParam)
{}

ACommand::~ACommand(void)
{}

void ACommand::recv(Message& message)
{
	senderUser = dynamic_cast<User*>(message.getSender());
	senderServer = dynamic_cast<Server*>(message.getSender());
	senderUnknown = dynamic_cast<Unknown*>(message.getSender());

	if (senderUser && (senderUser->getLevel() & this->levelAccess || senderUser->isOper()))
	{
		if (message.size() < this->minParam)
		{
			Numeric::insertField(message.getCmd());
			message.replyNumeric(ERR_NEEDMOREPARAMS);
		}
		else if (!this->_recvUser(message))
		{
			Numeric::insertField(message.getCmd());
			message.replyNumeric(ERR_NOTIMPLEMENTED);
		}
	}
	else if (senderUnknown && senderUnknown->getLevel() & this->levelAccess)
	{
		if (message.size() < this->minParam)
		{
			Numeric::insertField(message.getCmd());
			message.replyNumeric(ERR_NEEDMOREPARAMS);
		}
		else if (!this->_recvUnknown(message))
		{
			Numeric::insertField(message.getCmd());
			message.replyNumeric(ERR_NOTIMPLEMENTED);
		}
	}
	else if (senderServer && senderServer->getLevel() & this->levelAccess)
	{
		if (!this->_recvServer(message))
		{
			Numeric::insertField(message.getCmd());
			message.replyNumeric(ERR_NOTIMPLEMENTED);
		}
	}
	else if (this->levelAccess & (LEVEL_REGISTERED | LEVEL_NEGOTIATING))
	{
		Numeric::insertField(message.getCmd());
		message.replyNumeric(ERR_NOTREGISTERED);
	}
	else if (this->levelAccess == LEVEL_UNREGISTERED)
	{
		Numeric::insertField(message.getCmd());
		message.replyNumeric(ERR_ALREADYREGISTERED);
	}
	else if (this->levelAccess == LEVEL_IRCOPERATOR)
	{
		Numeric::insertField(message.getCmd());
		message.replyNumeric(ERR_NOPRIVILEGES);
	}
}

void ACommand::send(Message& message)
{
	receiverUser = dynamic_cast<User*>(message.getReceiver());
	receiverServer = dynamic_cast<Server*>(message.getReceiver());

	if (receiverUser)
		this->_sendUser(message);
	else if (receiverServer)
		this->_sendServer(message);
}
