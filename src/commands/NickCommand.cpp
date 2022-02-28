#include "NickCommand.hpp"
#include "User.hpp"
#include "Delegate.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include "ChanModeConfig.hpp"

#include <iostream>
#include <map>
#include <cctype>

NickCommand::NickCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

NickCommand::~NickCommand(void)
{}

void	NickCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	Delegate<NickCommand, Message> *nickDelegate = new Delegate<NickCommand, Message>(*this, &NickCommand::nickEvent);

	eventHandler.add(NICKEVENT, *nickDelegate);
}

void	NickCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void	NickCommand::nickEvent(Message& message)
{
	std::cout << "<" << message.getSender()->getName() << "> papa" << std::endl; 
}

bool	NickCommand::_recvUser(Message& message)
{
	User*									user = this->userSender;
	std::string								oldName = user->getName();
	std::string								newName = message[0];
	User*									newUser;
	std::map<std::string, User* >&			userMap = this->server.getUserMap();
	Server::userVector_type*				uniqueUsers;

	message.setReceiver(message.getSender());
	// valida nick
	if (newName.size() > MAXNICK || !isValidNick(newName))
	{
		Numeric::insertField(newName);
		message.send(Numeric::builder(message, ERR_ERRONEUSNICKNAME));
		return true;
	}
	if (oldName == newName)
		return true;
	// si el nick coincide sin respetar mayusculas/misnusculas
	// Es este caso el nick aunque no ha cambiado, si cambia de "estética" y se considera un cambio de nick a todos los efectos.
	// Hay que notificar al resto de usuarios que compartan canal y a él mismo que el nick ha cambiado.
	if (strToUpper(oldName) == strToUpper(newName))
	{
		message.hideReceiver();
		message.send();
		user->setName(newName);
		return true;
	}
	if (!(newUser = this->server.userAt(newName)))
	{
		if (oldName.empty() && user->getIdent() != "anonymous")
		{
			if (!this->server.getPass().empty() && user->getPass() != this->server.getPass())
			{
				message.limitMaxParam(1);
				message.setCmd("QUIT");
				message[0] = "Password incorrect";
				message.process();
				return true;
			}
			user->setStatus(LEVEL_NEGOTIATING);
			user->setPingChallenge("challenge-string");
			user->send("PING :" + user->getPingChallenge());
		}
		if (!oldName.empty())
		{
			for (Server::channelMap_iterator it = user->begin(); it != user->end(); ++it)
			{
				message.setChannel(it->second);
				if (!server.checkChannelMode(message, COMMAND_NICK))
					return true;
			}
			userMap.erase(strToUpper(oldName));
			if ((user->getStatus() & (LEVEL_REGISTERED | LEVEL_IRCOPERATOR)))
			{
				uniqueUsers = user->getUniqueVector();
				message.setReceiver(*uniqueUsers);
				message.hideReceiver();
				message.send();
				delete uniqueUsers;
			}
			for (Server::channelMap_iterator it = user->begin(); it != user->end(); ++it)
			{
				it->second->erase(oldName);
				it->second->insert(newName, user);
			}
		}
		user->setName(newName);
		userMap[strToUpper(newName)] = user;
	}
	else
	{
		Numeric::insertField(newName);
		message.replyNumeric(ERR_NICKNAMEINUSE);
	}
	return true;	
}

bool	NickCommand::_recvServer(Message& message)
{
	(void)message;
	return false;
}
bool	NickCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool	NickCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
