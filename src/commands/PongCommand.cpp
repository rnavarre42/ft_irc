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
	message.limitMaxParam(0);
	message.setReceiver(this->userSender);
	message.insertField("");
	message.setCmd("MODE");
	message.send();

	Numeric::insertField(this->userSender->getMask());
	message.send(Numeric::builder(message, RPL_WELCOME));
	
	Numeric::insertField(message.getSender()->getMask());
	message.send(Numeric::builder(message, RPL_YOURHOST));
	
	message.send(Numeric::builder(message, RPL_CREATED));
	
	Numeric::insertField(this->server.getName());
	Numeric::insertField("iO");
	Numeric::insertField("beiklmnostv");
	message.send(Numeric::builder(message, RPL_MYINFO));

	this->server.supportNames();
//	Numeric::insertField("AWAYLEN=200 CASEMAPPING=ascii CHANLIMIT=#:3 CHANMODES=be,k,inst CHANNELLEN=64 CHANTYPES=# EXCEPTS=e HOSTLEN=64 KEYLEN=32");
	message.send(Numeric::builder(message, RPL_ISUPPORT));

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
