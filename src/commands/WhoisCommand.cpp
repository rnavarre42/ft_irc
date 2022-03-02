#include "WhoisCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include "User.hpp"

#include <iostream>

WhoisCommand::WhoisCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

WhoisCommand::~WhoisCommand(void)
{}

void	WhoisCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void	WhoisCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

bool	WhoisCommand::_recvUser(Message& message)
{
	User&			user = *this->userSender;
	User*			targetUser;
	std::string&	target = message[0];
	std::string		prefix;

	(void)message;
	(void)user;

	if ((targetUser = server.userAt(target)))
	{		
		Numeric::insertField(targetUser->getName());
		Numeric::insertField(targetUser->getIdent());
		Numeric::insertField(targetUser->getHost());	//should be encrypted host but... not implemented.
		Numeric::insertField(targetUser->getReal());
		message.replyNumeric(RPL_WHOISUSER);

		Numeric::insertField(targetUser->getName());
		Numeric::insertField(targetUser->getMask());
		Numeric::insertField(targetUser->getHost());	//real host/ip (unencrypted)
		message.replyNumeric(RPL_WHOISHOST);

		if (targetUser->size())
		{
			Numeric::insertField(targetUser->getName());
			for (Server::channelMap_iterator chanIt = targetUser->begin()
					; chanIt != targetUser->end()
					; ++chanIt)
			{
				if (chanIt->second->isOper(targetUser))
					prefix += "@";
				if (chanIt->second->isVoice(targetUser))
					prefix += "+";
				Numeric::insertField(prefix + chanIt->second->getName());
				prefix.clear();
			}
			message.replyNumeric(RPL_WHOISCHANNELS);
		}

		Numeric::insertField(targetUser->getName());
		Numeric::insertField(server.getName());
		Numeric::insertField(server.getReal());
		message.replyNumeric(RPL_WHOISSERVER);
		
		if (targetUser->isAway())
		{
			Numeric::insertField(targetUser->getName());
			Numeric::insertField(targetUser->getAwayMsg());
			message.replyNumeric(RPL_AWAY);
		}

		if (targetUser->isOper())
		{
			Numeric::insertField(targetUser->getName());
			Numeric::insertField("O");
			message.replyNumeric(RPL_WHOISMODES);
		}

		Numeric::insertField(targetUser->getName());
		Numeric::insertField(to_string(std::time(NULL) - targetUser->getIdleTime()));
		Numeric::insertField(to_string(targetUser->getSignTime()));
		message.replyNumeric(RPL_WHOISIDLE);

		if (targetUser->isOper())
		{
			Numeric::insertField(targetUser->getName());
			message.replyNumeric(RPL_WHOISOPERATOR);
		}

		Numeric::insertField(targetUser->getName());
		message.replyNumeric(RPL_ENDOFWHOIS);
	}
	else
	{
		Numeric::insertField(target);
		message.replyNumeric(ERR_NOSUCHNICK);
		Numeric::insertField(target);
		message.replyNumeric(RPL_ENDOFWHOIS);
	}

	return true;
}

bool	WhoisCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool	WhoisCommand::_sendUser(Message& message)
{
	User*	user = this->userReceiver;

	(void)message;
	(void)user;
	return false;
}

bool	WhoisCommand::_sendServer(Message& message)
{
	Server*	server = this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
