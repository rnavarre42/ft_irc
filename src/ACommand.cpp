#include "ACommand.hpp"
#include "numerics.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include <string>
#include <iostream>

ACommand::ACommand(Server &server, int level) : server(server), level(level)
{}

ACommand::~ACommand(void)
{}

void ACommand::exec(Message &message)
{
	if (level == LEVEL_ALL || message.getSender().isRegistered() == level)
		this->_exec(message);
	else if (level == LEVEL_REGISTERED)
		message.getSender().send(Numeric::builder(this->server, message, ERR_NOTREGISTERED));
	else if (level == LEVEL_UNREGISTERED)
		message.getSender().send(Numeric::builder(this->server, message, ERR_ALREADYREGISTERED));
}
