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

void AwayCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void AwayCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

bool AwayCommand::_recvUser(Message& message)
{
	User*	user = this->userSender;

	if (!message.size() || message[0].empty())
	{
		user->getAwayMsg().empty();
		user->send(Numeric::builder(this->server, *user, RPL_UNAWAY));
	}
	else //if (message[0] != user.getAwayMsg())
	{
		user->setAwayMsg(message[0]);
		user->send(Numeric::builder(this->server, *user, RPL_NOAWAY));
	}	
	return true;
}

bool AwayCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool AwayCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool AwayCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
