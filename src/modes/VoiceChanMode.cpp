#include "VoiceChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Console.hpp"
#include "User.hpp"

VoiceChanMode::VoiceChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'v';
	this->_chanModeConfig.events = 0;
	this->_chanModeConfig.unique = false;
}

VoiceChanMode::~VoiceChanMode(void)
{}

void	VoiceChanMode::onChanEvent(Access& access, int event, Message &message, int& numeric)
{
	(void)access;
	(void)event;
	(void)message;
	(void)numeric;
}

bool	VoiceChanMode::onChanModeEvent(int pos, int sign, Channel& channel, Message& message)
{
	Server::userMap_iterator	targetIt;

	targetIt = channel.find(message[pos]);
	if (targetIt == channel.end())
	{
		Numeric::insertField(message[pos]);
		message.replyNumeric(ERR_NOSUCHNICK);
	}
	else
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

void	VoiceChanMode::onShowChanModeEvent(void)
{}

void	VoiceChanMode::onDelete(void* )
{}

std::string VoiceChanMode::toString(void* pointer)
{
	return reinterpret_cast<User* >(pointer)->getName();
}
