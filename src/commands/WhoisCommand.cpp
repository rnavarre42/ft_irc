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
// RPL_WHOISUSER		311 <nick> <ident> <host> * :<realname>
// RPL_WHOISHOST		378 <nick> :is connecting from <user mask encrypted> <real ip>
// RPL_WHOISCHANNELS	319 <nick> :<@+#channel>			(condicion)
// RPL_WHOISSERVER		312 <nick> <server> :<name>
// RPL_AWAY				301 <nick> <away message>			(condicion)
// RPL_WHOISMODES		379 <nick> :is using modes +xxx
// RPL_WHOISIDLE		317 <nick> idle signon :seconds idle, signon time
// RPL_WHOISOPERATOR	313 <nick> :is an IRC Operator		(condicion)
// RPL_WHOISREGNICK		307 <nick> :es un nick registrado	(condicion)
// RPL_ENDOFWHOIS		318 <nick> :End of /WHOIS list.
//
bool	WhoisCommand::_recvUser(Message& message)
{
	User&			user = *this->senderUser;
	User*			targetUser;
	std::string&	target = message[0];

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
		Numeric::insertField(targetUser->getHost()); //real host/ip (unencrypted)
		message.replyNumeric(RPL_WHOISHOST);

		Numeric::insertField(targetUser->getName());
		if (targetUser->size())
		{
			for (Server::channelMap_iterator chanIt = targetUser->begin()
					; chanIt != targetUser->end()
					; ++chanIt)
				Numeric::insertField(chanIt->second->getName());
			message.replyNumeric(RPL_WHOISCHANNELS);
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
	Server&	server = *this->senderServer;

	(void)message;
	(void)server;
	return false;
}

bool	WhoisCommand::_recvUnknown(Message&)
{
	return false;
}

bool	WhoisCommand::_sendUser(Message& message)
{
	User*	user = this->receiverUser;

	(void)message;
	(void)user;
	return false;
}

bool	WhoisCommand::_sendServer(Message& message)
{
	Server*	server = this->receiverServer;

	(void)message;
	(void)server;
	return false;
}
