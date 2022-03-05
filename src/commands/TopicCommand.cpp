#include "TopicCommand.hpp"
#include "ChanModeConfig.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Server.hpp"
#include "ChanModeConfig.hpp"
			
#include <iostream>

TopicCommand::TopicCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

TopicCommand::~TopicCommand(void)
{}

void	TopicCommand::loadEvents(Server::eventHandler_type&)
{}

void	TopicCommand::unloadEvents(Server::eventHandler_type&)
{}

bool	TopicCommand::_recvUser(Message& message)
{
	User*						user = this->senderUser;
	std::string					target;
	std::string					newTopic;
	Channel*					channel;
	const Channel::TopicInfo*	topicInfo;

	target = message[0];
	if (!(channel = server.channelAt(target)))
	{
		Numeric::insertField(target);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
	}
	else
	{
		topicInfo = &channel->getTopicInfo();
		newTopic = message[1];
		message.setChannel(channel);
		if (message.size() == 1)
		{
			if (topicInfo->topic.empty())
			{
				Numeric::insertField(channel->getName());
				message.replyNumeric(RPL_NOTOPIC);
			}
			else
			{
				Numeric::insertField(channel->getName());
				Numeric::insertField(topicInfo->topic);
				message.replyNumeric(RPL_TOPIC);
				Numeric::insertField(channel->getName());
				Numeric::insertField(topicInfo->own);
				Numeric::insertField(topicInfo->time);
				message.replyNumeric(RPL_TOPICTIME);
			}
		}
		else
		{
			if (!server.checkChannelMode(message, COMMAND_TOPIC))
				return true;
			if (topicInfo->topic != newTopic)
			{
				channel->setTopicInfo(user->getName(), newTopic);
				message.setReceiver(channel);
				message.setReceiver(user);
				message.send();
			}
		}
	}
	return true;
}

bool	TopicCommand::_recvServer(Message&)
{
	return false;
}

bool	TopicCommand::_sendUser(Message&)
{
	return false;
}

bool	TopicCommand::_sendServer(Message&)
{
	return false;
}
