#include "PassCommand.hpp"
#include "User.hpp"
#include "Unknown.hpp"
#include "Message.hpp"
#include "Server.hpp"

#include <iostream>

PassCommand::PassCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

PassCommand::~PassCommand(void)
{}

void	PassCommand::loadEvents(Server::eventHandler_type&)
{}

void	PassCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	PassCommand::_recvUnknown(Message& message)
{
	Unknown&	unknown = *this->_senderUnknown;

	unknown.setPass(message[0]);
	return true;
}

bool	PassCommand::_recvUser(Message&)
{
	return true;
}

bool	PassCommand::_recvServer(Message&)
{
	return false;
}

bool	PassCommand::_sendUnknown(Message&)
{
	return false;
}

bool	PassCommand::_sendUser(Message&)
{
	return false;
}

bool	PassCommand::_sendServer(Message&)
{
	return false;
}
