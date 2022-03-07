#include "AwayCommand.hpp"
#include "User.hpp"
#include "Numeric.hpp"
#include "Message.hpp"
#include "Server.hpp"

#include <iostream>

AwayCommand::AwayCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

AwayCommand::~AwayCommand(void)
{}

void	AwayCommand::loadEvents(Server::eventHandler_type&)
{}

void	AwayCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	AwayCommand::_recvUser(Message& message)
{
	User*	user = this->_senderUser;

	if (!message.size() || message[0].empty())
	{
		user->getAwayMsg().empty();
		user->send(Numeric::builder(this->_server, *user, RPL_UNAWAY));
	}
	else //if (message[0] != user.getAwayMsg())
	{
		user->setAwayMsg(message[0]);
		user->send(Numeric::builder(this->_server, *user, RPL_NOAWAY));
	}	
	return true;
}

bool	AwayCommand::_recvServer(Message&)
{
	return false;
}

bool	AwayCommand::_recvUnknown(Message&)
{
	return false;
}

bool	AwayCommand::_sendUser(Message&)
{
	return false;
}

bool	AwayCommand::_sendServer(Message&)
{
	return false;
}

bool	AwayCommand::_sendUnknown(Message&)
{
	return false;
}
