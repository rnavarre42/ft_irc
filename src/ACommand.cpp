#include "ACommand.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Unknown.hpp"
#include "User.hpp"

#include <string>
#include <iostream>

ACommand::ACommand(Server& server, int levelAccess, int minParam)
	: _server(server)
	, _levelAccess(levelAccess)
	, _minParam(minParam)
{}

ACommand::~ACommand(void)
{}

static void	needMoreParams(Message& message)
{
	Numeric::insertField(message.getCmd());
	message.replyNumeric(ERR_NEEDMOREPARAMS);
}

static void	notImplemented(Message& message)
{
	Numeric::insertField(message.getCmd());
	message.replyNumeric(ERR_NOTIMPLEMENTED);
}


static void	notRegistered(Message& message)
{
	Numeric::insertField(message.getCmd());
	message.replyNumeric(ERR_NOTREGISTERED);
}

static void	alreadyRegistered(Message& message)
{
	Numeric::insertField(message.getCmd());
	message.replyNumeric(ERR_ALREADYREGISTERED);
}

static void	noPrivileges(Message& message)
{
	Numeric::insertField(message.getCmd());
	message.replyNumeric(ERR_NOPRIVILEGES);
}

void ACommand::recv(Message& message)
{
	this->_senderUser = dynamic_cast<User*>(&message.getSender());
	this->_senderServer = dynamic_cast<Server*>(&message.getSender());
	this->_senderUnknown = dynamic_cast<Unknown*>(&message.getSender());

	if (message.size() < this->_minParam)
		needMoreParams(message);
	else if (this->_senderUser && (this->_senderUser->getLevel() & this->_levelAccess || this->_senderUser->isOper()))
	{
		if (!this->_recvUser(message))
			notImplemented(message);
	}
	else if (this->_senderUnknown && this->_senderUnknown->getLevel() & this->_levelAccess)
	{
		if (!this->_recvUnknown(message))
			notImplemented(message);
	}
	else if (this->_senderServer && this->_senderServer->getLevel() & this->_levelAccess)
	{
		if (!this->_recvServer(message))
			notImplemented(message);
	}
	else if (this->_levelAccess & (LEVEL_REGISTERED | LEVEL_NEGOTIATING))
		notRegistered(message);
	else if (this->_levelAccess == LEVEL_UNREGISTERED)
		alreadyRegistered(message);
	else if (this->_levelAccess == LEVEL_IRCOPERATOR)
		noPrivileges(message);
}

void ACommand::send(Message& message)
{
	this->_receiverUser = dynamic_cast<User*>(&message.getReceiver());
	this->_receiverServer = dynamic_cast<Server*>(&message.getReceiver());
	this->_receiverUnknown = dynamic_cast<Unknown*>(&message.getReceiver());

	if (this->_receiverUser)
		this->_sendUser(message);
	else if (this->_receiverServer)
		this->_sendServer(message);
	else if (this->_receiverUnknown)
		this->_sendUnknown(message);
}
