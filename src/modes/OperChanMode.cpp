#include "OperChanMode.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Console.hpp"

OperChanMode::OperChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'o';
	this->_chanModeConfig.events = CHANMODE_KICK | CHANMODE_MODE | CHANMODE_INVITE;
	this->_chanModeConfig.unique = false;
}

OperChanMode::~OperChanMode(void)
{}

void	OperChanMode::onChanEvent(Access& access, int event, Message& message, int& numeric)
{
	Channel*					channel = message.getChannel();
	Server::userMap_iterator	kickUserIt;
	(void)event;

	if (this->isSetMode(*channel, message.getSender()))
	{
		if ((kickUserIt = this->_server.userFind(message[1])) == this->_server.getUserMap().end())
			Numeric::insertField(channel->getName());
	}
	else
	{
		Numeric::insertField(channel->getName());
		numeric = ERR_CHANOPRIVSNEEDED;
		access = AChanMode::deny;
	}
}

bool	OperChanMode::onChanModeEvent(int pos, int sign, Channel& channel, Message& message)
{
	Server::userMap_iterator	targetIt;
	
	targetIt = channel.find(message[pos]);
	if (targetIt == channel.end())
	{
		Numeric::insertField(message[pos]);
		message.replyNumeric(ERR_NOSUCHNICK);
	}
	else	// Aquí se añade el modo a la lista del canal
	{
		if ((sign && this->setMode(channel, targetIt->second))
				|| (!sign && this->unsetMode(channel, targetIt->second)))
		{
			message[pos] = targetIt->second->getName();
			return true;
		}
	}
	return false;
}

void	OperChanMode::onShowChanModeEvent(void)
{
}

void	OperChanMode::onDelete(void* )
{
}

std::string OperChanMode::toString(void* pointer)
{
	return reinterpret_cast<User* >(pointer)->getName();
}
