#include "NickCommand.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <iostream>
#include <map>

NickCommand::NickCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

bool NickCommand::_execUser(Message &message)
{
	User &user = *this->userSender;
	std::string								oldName = user.getName();
	std::string								newName = *message.getParam(0);
	std::map<std::string, User *>::iterator	it;
	std::map<std::string, User *>			&userMap = this->server.getUserMap();

	// si el nick coincide respetando las mayusculas/minusculas, se ignora.
	if (oldName == newName)
		return true;
	// si el nick coincide sin respetar mayusculas/misnusculas
	// Es este caso el nick aunque no ha cambiado, si cambia de "estética" y se considera un cambio de nick a todos los efectos.
	// Hay que notificar al resto de usuarios que compartan canal y a él mismo que el nick ha cambiado.
	if (strToUpper(oldName) == strToUpper(newName))
	{
		user.send(":" + user.getMask() + " NICK :" + newName);
		user.setName(newName);
		return true;
	}
	it = userMap.find(strToUpper(newName));
	if (it == userMap.end())
	{
		if (oldName.empty() && !user.getIdent().empty())
		{
			user.setPingChallenge("challenge-string");
			user.send("PING :" + user.getPingChallenge());
		}
		if (!oldName.empty())
		{
			userMap.erase(oldName);
			user.send(":" + user.getMask() + " NICK :" + newName);
		}
		user.setName(newName);
		userMap[newName] = &user;
	}
	else
		user.send(Numeric::builder(this->server, user, ERR_NICKNAMEINUSE, (std::string[]){*message.getParam(0)}, 1));
	return true;	
}

bool NickCommand::_execServer(Message &message)
{
	(void)message;
	return false;
}
