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
	if (this->level == LEVEL_ALL || message.getSender().isRegistered() == level)
	{
		if (this->count < message.getCount())
			message.getSender().send(Numeric::builder(this->server, message, ERR_NEEDMOREPARAMS, (std::string[]){message.getCmd()}, 1));
		else
			this->_exec(message);
	}
	else if (this->level == LEVEL_REGISTERED)
		message.getSender().send(Numeric::builder(this->server, message, ERR_NOTREGISTERED));
	else if (this->level == LEVEL_UNREGISTERED)
		message.getSender().send(Numeric::builder(this->server, message, ERR_ALREADYREGISTERED));
}
