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

	message.getSender().setIdleTime(time(NULL));
	if (this->level == LEVEL_ALL || message.getSender().isRegistered() == level || message.getSender().isOper())
	{
		if (message.size() < this->count)
		{
			Numeric::insertField(message.getCmd());
			message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_NEEDMOREPARAMS));
		}
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
			{
				Numeric::insertField(message.getCmd());
				message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_NOTIMPLEMENTED));
			}
		}
	}
	else if (this->level == LEVEL_REGISTERED)
		message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_NOTREGISTERED));
	else if (this->level == LEVEL_UNREGISTERED)
		message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_ALREADYREGISTERED));
	else if (this->level == LEVEL_IRCOPERATOR)
		message.getSender().send(Numeric::builder(this->server, message.getSender(), ERR_NOPRIVILEGES));
}
