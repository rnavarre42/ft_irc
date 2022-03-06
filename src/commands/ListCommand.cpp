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

void	ListCommand::loadEvents(Server::eventHandler_type&)
{
}

void	ListCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	ListCommand::_recvUser(Message& message)
{
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

bool	ListCommand::_recvServer(Message&)
{
	return false;
}

bool	ListCommand::_recvUnknown(Message&)
{
	return false;
}

bool	ListCommand::_sendUser(Message&)
{
	return false;
}

bool	ListCommand::_sendServer(Message&)
{
	return false;
}
