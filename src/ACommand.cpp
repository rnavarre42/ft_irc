#include "ACommand.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
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
	bool	ret = true;

	senderUser = dynamic_cast<User*>(message.getSender());
	senderServer = dynamic_cast<Server*>(message.getSender());

	if (senderUser->getLevel() & this->levelAccess || (senderUser && senderUser->isOper()))
	{
		if (message.size() < this->minParam)
		{
			Numeric::insertField(message.getCmd());
			message.replyNumeric(ERR_NEEDMOREPARAMS);
		}
		else
		{
			if (senderUser)
				ret = this->_recvUser(message);
			else if (senderServer)
				ret = this->_recvServer(message);
			if (!ret)
			{
				Numeric::insertField(message.getCmd());
				message.replyNumeric(ERR_NOTIMPLEMENTED);
			}
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
