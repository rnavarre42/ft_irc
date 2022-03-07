#include "PongCommand.hpp"
#include "User.hpp"
#include "Unknown.hpp"
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
	Console::log(LOG_INFO, "El usuario " + message.getSender().getName() + " se ha registrado");	
	message.limitMaxParam(0);
	message.setReceiver(*this->_senderUser);
	message.insertField("");
	message.setCmd("MODE");
	message.send();

	Numeric::insertField(this->_senderUser->getMask());
	message.send(Numeric::builder(message, RPL_WELCOME));
	
	Numeric::insertField(message.getSender().getMask());
	message.send(Numeric::builder(message, RPL_YOURHOST));
	
	message.send(Numeric::builder(message, RPL_CREATED));
	
	Numeric::insertField(this->_server.getName());
	Numeric::insertField("iO");
	Numeric::insertField("beiklmnostv");
	message.send(Numeric::builder(message, RPL_MYINFO));

	Numeric::insertField("AWAYLEN=200 CASEMAPPING=ascii CHANLIMIT=#:3 CHANMODES=be,k,inst CHANNELLEN=64 CHANTYPES=# EXCEPTS=e HOSTLEN=64 KEYLEN=32");
	message.send(Numeric::builder(message, RPL_ISUPPORT));

	message.setCmd("MOTD");
	message.internal();
}

bool	PongCommand::_recvUnknown(Message& message)
{
	Unknown&	unknown = *this->_senderUnknown;

	if (message[0] == unknown.getPingChallenge())
	{
		this->_server.setSenderLevel(unknown, LEVEL_REGISTERED);
		unknown.clearPingChallenge();
		unknown.setNextTimeout(0);
	}
	else
	{
		message.limitMaxParam(1);
		message.setCmd("QUIT");
		message[0] = "Incorrect ping reply for registration";
		message.internal();
	}
	return true;
}

bool	PongCommand::_recvUser(Message& message)
{
	User&	user = *this->_senderUser;

	if (message[0] == user.getPingChallenge())
	{
		user.clearPingChallenge();
		user.setNextTimeout(0);
	}
	return true;
}

bool	PongCommand::_recvServer(Message&)
{
	return false;
}

bool	PongCommand::_sendUnknown(Message&)
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
