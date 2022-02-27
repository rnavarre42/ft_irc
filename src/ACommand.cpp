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

	if (message.getSender()->getStatus() & this->levelAccess || message.getSender()->isOper())
	{
		if (message.size() < this->minParam)
		{
			Numeric::insertField(message.getCmd());
			message.getSender()->send(Numeric::builder(this->server, *message.getSender(), ERR_NEEDMOREPARAMS));
		}
		else
		{
			if (message.getSender()->isUser())
			{
				this->userSender = static_cast<User *>(message.getSender());
				ret = this->_recvUser(message);
			}
			else if (message.getSender()->isServer())
			{
				this->serverSender = static_cast<Server *>(message.getSender());
				ret = this->_recvServer(message);
			}
			if (!ret)
			{
				Numeric::insertField(message.getCmd());
				message.getSender()->send(Numeric::builder(this->server, *message.getSender(), ERR_NOTIMPLEMENTED));
			}
		}
	}
	else if (this->levelAccess & (LEVEL_REGISTERED | LEVEL_NEGOTIATING))
	{
		Numeric::insertField(message.getCmd());
		message.getSender()->send(Numeric::builder(this->server, *message.getSender(), ERR_NOTREGISTERED));
	}
	else if (this->levelAccess == LEVEL_UNREGISTERED)
	{
		Numeric::insertField(message.getCmd());
		message.getSender()->send(Numeric::builder(this->server, *message.getSender(), ERR_ALREADYREGISTERED));
	}
	else if (this->levelAccess == LEVEL_IRCOPERATOR)
	{
		Numeric::insertField(message.getCmd());
		message.getSender()->send(Numeric::builder(this->server, *message.getSender(), ERR_NOPRIVILEGES));
	}
}

void ACommand::send(Message& message)
{
	if (message.getReceiver()->isUser())
	{
		this->userReceiver = static_cast<User *>(message.getReceiver());
		this->_sendUser(message);
	}
	else if (message.getReceiver()->isServer())
	{
		this->serverReceiver = static_cast<Server *>(message.getReceiver());
		this->_sendServer(message);
	}
}
