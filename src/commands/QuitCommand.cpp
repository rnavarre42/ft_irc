#include "QuitCommand.hpp"
#include "User.hpp"
#include "Unknown.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Console.hpp"
#include <iostream>

QuitCommand::QuitCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

QuitCommand::~QuitCommand(void)
{}

void	QuitCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	eventHandler.add(QUITEVENT, *new Delegate<QuitCommand, Message>(*this, &QuitCommand::quitEvent));
	eventHandler.add(DELUSEREVENT, *new Delegate<QuitCommand, Message>(*this, &QuitCommand::delUserEvent));
}

void	QuitCommand::unloadEvents(Server::eventHandler_type&)
{}

void	QuitCommand::delUserEvent(Message& message)
{
	ASender&		sender = message.getSender();

	if (sender.getName().empty())
		Console::log(LOG_INFO, "User <anonymous> disconnected (" + message[0] + ")");
	else
		Console::log(LOG_INFO, "User <" + sender.getName() + "> disconnected" + " (" + message[0] + ")");
	message.send("ERROR :Closing link: (" + sender.getIdent() + "@" + sender.getHost() + ") [" + message[0] + "]");
}

void	QuitCommand::quitEvent(Message& message)
{
	message.send();
}

bool	QuitCommand::_recvUnknown(Message& message)
{
	Unknown&	unknown = *this->senderUnknown;

	if (message.size())
		message[0].insert(0, "Quit: ");
	message.setCmd("QUIT");
	message.setReceiver(unknown);
	message.internal();
	std::cout << "que me voy" << std::endl;
	return true;
}

bool	QuitCommand::_recvUser(Message& message)
{
	User&	user = *this->senderUser;

	if (message.size())
		message[0].insert(0, "Quit: ");
	message.setCmd("QUIT");
	message.setReceiver(user);
	message.internal();
	return true;
}

bool	QuitCommand::_recvServer(Message&)
{
	return false;
}

bool	QuitCommand::_sendUser(Message& message)
{
	User*	user = this->receiverUser;
	
	if (!message.size())
		message.insertField("Client exited");
	this->server.deleteUser(*user, message[0]);
	return true;
}

bool	QuitCommand::_sendServer(Message&)
{
	return false;
}
