#include "QuitCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

QuitCommand::QuitCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void QuitCommand::loadEvents(Server::eventHandler_type &eventHandler)
{
	eventHandler.add(QUITEVENT, *new Delegate<QuitCommand, Source>(*this, &QuitCommand::QuitEvent));
}

void QuitCommand::unloadEvents(Server::eventHandler_type &eventHandler)
{
	(void)eventHandler;
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
	message.setReceiver(&user);
	server.sendCommand(message);
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
	user.send("ERROR :Closing link: (" + user.getMask() + ") [" + message[0] + "]");
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
