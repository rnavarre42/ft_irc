#include "QuitCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

QuitCommand::QuitCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void QuitCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	eventHandler.add(QUITEVENT, *new Delegate<QuitCommand, Source>(*this, &QuitCommand::QuitEvent));
	eventHandler.add(DELUSEREVENT, *new Delegate<QuitCommand, Source>(*this, &QuitCommand::DelUserEvent));
}

void QuitCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
}

void QuitCommand::DelUserEvent(Source &source)
{
	Message &message = *source.message;
	User	&user	= static_cast<User &>(*message.getSender());

	message.send("ERROR :Closing link: (" + user.getIdent() + "@" + user.getHost() + ") [" + message[0] + "]");
}

void QuitCommand::QuitEvent(Source &source)
{
	Message	&message = *source.message;

	message.send();
}

bool QuitCommand::_recvUser(Message &message)
{
	User	&user = *this->userSender;

	if (message.size())
		message[0].insert(0, "Quit: ");
	message.setCmd("QUIT");
	message.setSender(&this->server);
	message.setReceiver(&user);
//	this->server.sendCommand(message);
	message.process();
	return true;
}

bool QuitCommand::_recvServer(Message &message)
{
	Server	&server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool QuitCommand::_sendUser(Message &message)
{
	User	&user = *this->userReceiver;
	
	if (!message.size())
		message.insertField("Client exited");
	this->server.delUser(user, message[0]);
	return true;
}

bool QuitCommand::_sendServer(Message &message)
{
	Server	&server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
