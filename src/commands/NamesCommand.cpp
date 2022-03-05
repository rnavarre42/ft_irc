#include "NamesCommand.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Numeric.hpp"

#include <iostream>

NamesCommand::NamesCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}


NamesCommand::~NamesCommand(void)
{}

void	NamesCommand::loadEvents(Server::eventHandler_type&)
{}

void	NamesCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	NamesCommand::_recvUser(Message& message)
{
	Channel* 		channel;
	std::string&	target = message[0];
//	User*						user = this->userSender;

	message.setReceiver(message.getSender());
	if ((channel = this->server.channelAt(target)))
	{
		message.setChannel(channel);
		message.getServer()->names(*channel);
		message.replyNumeric(RPL_NAMREPLY);
	}
	Numeric::insertField(target);
	message.replyNumeric(RPL_ENDOFNAMES);
	return true;
}

bool	NamesCommand::_recvServer(Message&)
{
	return false;
}

bool	NamesCommand::_sendUser(Message& message)
{
	this->_recvUser(message);
	return false;
}

bool	NamesCommand::_sendServer(Message&)
{
	return false;
}
