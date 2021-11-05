#include "ACommand.hpp"
#include "numerics.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include <string>
#include <iostream>

ACommand::ACommand(Server &server, int accessLevel, int minParam) : server(server), accessLevel(accessLevel), minParam(minParam)
{}

ACommand::~ACommand(void)
{}

void ACommand::recv(Message &message)
{
	bool	ret = true;

	if (this->accessLevel == LEVEL_ALL || message.getSender().isRegistered() == accessLevel || message.getSender().isOper())
	{
		if (message.size() < this->minParam)
		{
			Numeric::insertField(message.getCmd());
			message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_NEEDMOREPARAMS));
		}
		else
		{
			if (message.getSender().isUser())
			{
				this->userSender = dynamic_cast<User *>(&message.getSender());
				ret = this->_recvUser(message);
			}
			else if (message.getSender().isServer())
			{
				this->serverSender = dynamic_cast<Server *>(&message.getSender());
				ret = this->_recvServer(message);
			}
			if (!ret)
			{
				Numeric::insertField(message.getCmd());
				message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_NOTIMPLEMENTED));
			}
		}
	}
	else if (this->accessLevel == LEVEL_REGISTERED)
		message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_NOTREGISTERED));
	else if (this->accessLevel == LEVEL_UNREGISTERED)
		message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_ALREADYREGISTERED));
	else if (this->accessLevel == LEVEL_IRCOPERATOR)
		message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_NOPRIVILEGES));
}

void ACommand::send(Message &message)
{
	if (message.getReceiver()->isUser())
	{
		this->userReceiver = dynamic_cast<User *>(message.getReceiver());
		this->_sendUser(message);
	}
	else if (message.getReceiver()->isServer())
	{
		this->serverReceiver = dynamic_cast<Server *>(message.getReceiver());
		this->_sendServer(message);
	}
}
