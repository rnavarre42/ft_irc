#include "ListCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"

#include <iostream>

ListCommand::ListCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

ListCommand::~ListCommand(void)
{}

void	ListCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void	ListCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

bool	ListCommand::_recvUser(Message& message)
{
//	User&	user = *this->userSender;

	for (Server::channelMap_iterator it = this->server.getChannelMap().begin()
			;it != this->server.getChannelMap().end()
			;++it)
	{
		Numeric::insertField(it->second->getName());
		Numeric::insertField(it->second->size());
		Numeric::insertField(it->second->getTopicInfo().topic);
		message.replyNumeric(RPL_LIST);
	}
	message.replyNumeric(RPL_LISTEND);
	
	return true;
}

bool	ListCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool	ListCommand::_sendUser(Message& message)
{
	User&	user = *this->userReceiver;
	
	(void)message;
	(void)user;
	return false;
}

bool	ListCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverReceiver;

	(void)message;
	(void)server;
	return false;
}
