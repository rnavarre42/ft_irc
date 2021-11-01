#include "ACommand.hpp"
#include "numerics.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include <string>
#include <iostream>

ACommand::ACommand(Server &server, int accessLevel, int paramCount) : server(server), level(accessLevel), count(paramCount)
{}

ACommand::~ACommand(void)
{}

void ACommand::exec(Message &message)
{
	bool	ret = true;

	if (this->level == LEVEL_ALL || message.getSender().isRegistered() == level || message.getSender().isOper())
	{
		if (message.getCount() < this->count)
			message.getSender().send(Numeric::builder(this->server, message, ERR_NEEDMOREPARAMS, (std::string[]){message.getCmd()}, 1));
		else
		{
			if (message.getSender().isUser())
			{
				this->userSender = dynamic_cast<User *>(&message.getSender());
				ret = this->_execUser(message);
			}
			if (message.getSender().isServer())
			{
				this->serverSender = dynamic_cast<Server *>(&message.getSender());
				ret = this->_execServer(message);
			}
			if (!ret)
				message.getSender().send(Numeric::builder(this->server, message, ERR_NOTIMPLEMENTED, (std::string[]){message.getCmd()}, 1));
		}
	}
	else if (this->level == LEVEL_REGISTERED)
		message.getSender().send(Numeric::builder(this->server, message, ERR_NOTREGISTERED));
	else if (this->level == LEVEL_UNREGISTERED)
		message.getSender().send(Numeric::builder(this->server, message, ERR_ALREADYREGISTERED));
	else if (this->level == LEVEL_IRCOPERATOR)
		message.getSender().send(Numeric::builder(this->server, message, ERR_NOPRIVILEGES));
}
