#include "TopicCommand.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Server.hpp"
#include <iostream>

TopicCommand::TopicCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

void	TopicCommand::loadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

void	TopicCommand::unloadEvents(Server::eventHandler_type& eventHandler)
{
	(void)eventHandler;
}

bool TopicCommand::_recvUser(Message& message)
{
	User*						user = this->userSender;
	Server::channelMap_iterator	chanIt = server.channelFind(message[0]);
	Channel*					channel = chanIt->second;
	const Channel::TopicInfo&	topicInfo = channel->getTopicInfo();

	if (chanIt == server.getChannelMap().end())
	{
		Numeric::insertField(message[0]);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
	}
	else
	{
		message.setChannel(channel);
		if (message.size() == 1)
		{
			if (topicInfo.topic.empty())
			{
				Numeric::insertField(channel->getName());
				message.replyNumeric(RPL_NOTOPIC);
			}
			else
			{
				Numeric::insertField(channel->getName());
				Numeric::insertField(topicInfo.topic);
				message.replyNumeric(RPL_TOPIC);
				Numeric::insertField(channel->getName());
				Numeric::insertField(topicInfo.own);
				Numeric::insertField(topicInfo.time);
				message.replyNumeric(RPL_TOPICTIME);
			}
		}
		else
		{
//			if (!channel.mode.raiseEvent(CHANMODE_TOPIC, message))
//			{
//				Numeric::insertField(channel->getName());
//				message.replyNueric(ERR_CHANOPRIVSNEEDED);
//			}
			if (topicInfo.topic != message[1])
			{
				channel->setTopicInfo(user->getName(), message[1]);
				message.setReceiver(channel);
				message.setReceiver(user);
				message.send();
			}
		}
	}
	return true;
}

bool TopicCommand::_recvServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}

bool TopicCommand::_sendUser(Message& message)
{
	User&	user = *this->userSender;

	(void)message;
	(void)user;
	return false;
}

bool TopicCommand::_sendServer(Message& message)
{
	Server&	server = *this->serverSender;

	(void)message;
	(void)server;
	return false;
}
