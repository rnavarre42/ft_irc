#include "ModerateChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"
#include "User.hpp"

ModerateChanMode::ModerateChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 'm';
	this->_chanModeConfig.events = COMMAND_PRIVMSG | COMMAND_NOTICE;
	this->_chanModeConfig.unique = true;
}

ModerateChanMode::~ModerateChanMode(void)
{}

void	ModerateChanMode::onChanEvent(Access& access, int, Message& message, int& numeric)
{
	Channel*	channel = message.getChannel();
	ASender*	user = message.getSender();

	if (access != AChanMode::deny && this->isSetMode(*channel) && !(channel->isVoice(user) || channel->isOper(user)))
	{
		Numeric::insertField(channel->getName());
		numeric = ERR_CANNOTSENDTOCHAN;
		access = AChanMode::deny;
	}
}

bool	ModerateChanMode::onChanModeEvent(int, int sign, Channel& channel, Message&)
{
	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	ModerateChanMode::onShowChanModeEvent(void)
{}

void	ModerateChanMode::onDelete(void*)
{}

std::string ModerateChanMode::toString(void*)
{
	return "";
}
