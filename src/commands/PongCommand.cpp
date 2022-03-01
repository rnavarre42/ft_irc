#include "PongCommand.hpp"
#include "User.hpp"
#include "Console.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Server.hpp"

#include <iostream>

PongCommand::PongCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

PongCommand::~PongCommand(void)
{}

void	PongCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	eventHandler.add(REGUSEREVENT, *new Delegate<PongCommand, Message>(*this, &PongCommand::registerUserEvent));
}

void	PongCommand::unloadEvents(Server::eventHandler_type&)
{}

void	PongCommand::registerUserEvent(Message& message)
{
	Console::log(LOG_INFO, "User <" + message.getSender()->getName() + "> is now registered");	
	Numeric::insertField(this->userSender->getHost());
	message.replyNumeric(RPL_VISIBLEHOST);

	message.limitMaxParam(0);
	message.setSender(message.getServer());
	message.setReceiver(this->userSender);
	message.insertField("");
	message.setCmd("MODE");
	message.send();

	message.setSender(message.getReceiver());
	Numeric::insertField(this->userSender->getMask());
	message.replyNumeric(RPL_WELCOME);
	
	Numeric::insertField(message.getServer()->getMask());
	message.replyNumeric(RPL_YOURHOST);
	
	message.replyNumeric(RPL_CREATED);
	
	Numeric::insertField(this->server.getName());
	Numeric::insertField("io");
	Numeric::insertField("beiklmnostv");
	Numeric::insertField("beklov");
	message.replyNumeric(RPL_MYINFO);

	this->server.supportNames();
	message.replyNumeric(RPL_ISUPPORT);

	message.setCmd("MOTD");
	message.process();
}

bool	PongCommand::_recvUser(Message& message)
{
	User*	user = this->userSender;

	if (message[0] == user->getPingChallenge())
	{
		if (user->getStatus() & ~LEVEL_REGISTERED)
			this->server.setSenderStatus(*user, LEVEL_REGISTERED);
		user->clearPingChallenge();
		user->setNextTimeout(0);
	}
	else if (user->getStatus() & ~LEVEL_REGISTERED)
	{
		message.limitMaxParam(1);
		message.setCmd("QUIT");
		message[0] = "Incorrect ping reply for registration";
		message.process();
	}
	message.getSender()->setIdleTime(time(NULL));
	return true;
}

bool	PongCommand::_recvServer(Message&)
{
	return false;
}
bool	PongCommand::_sendUser(Message&)
{
	return false;
}

bool	PongCommand::_sendServer(Message&)
{
	return false;
}
