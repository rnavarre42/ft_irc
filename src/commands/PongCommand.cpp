#include "PongCommand.hpp"
#include "User.hpp"
#include "Console.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Source.hpp"
#include "Server.hpp"
#include <iostream>

PongCommand::PongCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void PongCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	eventHandler.add(REGUSEREVENT, *new Delegate<PongCommand, Message>(*this, &PongCommand::registerUserEvent));
}

void PongCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void PongCommand::registerUserEvent(Message &message)
{
//	Message &message = *source.message;

	Console::log(LOG_INFO, "El usuario " + message.getSender()->getName() + " se ha registrado");	
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
	Numeric::insertField("i");
	Numeric::insertField("iknst");
	message.send(Numeric::builder(message, RPL_MYINFO));

	Numeric::insertField("AWAYLEN=200 CASEMAPPING=ascii CHANLIMIT=#:3 CHANMODES=be,k,inst CHANNELLEN=64 CHANTYPES=# EXCEPTS=e HOSTLEN=64 KEYLEN=32");
	message.send(Numeric::builder(message, RPL_ISUPPORT));

	message.setCmd("MOTD");
	message.process();
//	
//	this->server.sendCommand(message);
}

bool PongCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	if (message[0] == user.getPingChallenge())
	{
		if (user.getStatus() & ~LEVEL_REGISTERED)
			this->server.setSenderStatus(user, LEVEL_REGISTERED);
		user.clearPingChallenge();
		user.setNextTimeout(0);
	}
	else if (user.getStatus() & ~LEVEL_REGISTERED)
	{
		message.limitMaxParam(1);
		message.setCmd("QUIT");
		message[0] = "Incorrect ping reply for registration";
		message.process();
//		this->server.sendCommand(message);
//		this->server.delUser(user);
	}
//		server.killUser(user, "You should be dead now!");
	return true;
}

bool PongCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
bool PongCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool PongCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
