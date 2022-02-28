#include "NoExternalChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Numeric.hpp"

NoExternalChanMode::NoExternalChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 'n';
	this->_chanModeConfig.events = COMMAND_PRIVMSG | COMMAND_PRIVMSG;
	this->_chanModeConfig.unique = true;
}

NoExternalChanMode::~NoExternalChanMode(void)
{}

void	NoExternalChanMode::onChanEvent(Access& access, int, Message& message, int& numeric)
{
	Channel*	channel = message.getChannel();
	User*		user  = static_cast<User*>(message.getSender());

	if (access != AChanMode::deny && this->isSetMode(*channel) && !user->isOnChannel(*channel))
	{
		Numeric::insertField(channel->getName());
		numeric = ERR_CANNOTSENDTOCHAN;
		access = AChanMode::deny;
	}
}

bool	NoExternalChanMode::onChanModeEvent(int, int sign, Channel& channel, Message&)
{
	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	NoExternalChanMode::onShowChanModeEvent(void)
{}

void	NoExternalChanMode::onDelete(void*)
{}

std::string	NoExternalChanMode::toString(void*)
{
	return "";
}
