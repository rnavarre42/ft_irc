#include "InviteChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"

InviteChanMode::InviteChanMode(Server& server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 'i';
	this->_chanModeConfig.events = COMMAND_JOIN;
	this->_chanModeConfig.unique = true;
}

InviteChanMode::~InviteChanMode(void)
{}

void	InviteChanMode::onChanEvent(Access& access, int event, Message& message, int& numeric)
{
	Channel*	channel = message.getChannel();
	(void)event;

	if (this->isSetMode(*channel))
	{
		if ((this->_server.invite().find(message.getSender(), channel)) == this->_server.invite().end())
		{
			if (access != AChanMode::deny)
			{
				Numeric::insertField(channel->getName());
				numeric = ERR_INVITEONLYCHAN;
				access = AChanMode::deny;
			}
		}
		else
			access = AChanMode::allow;
	}
	else if ((this->_server.invite().find(message.getSender(), channel)) != this->_server.invite().end())
		access = AChanMode::allow;
}

bool	InviteChanMode::onChanModeEvent(int pos, int sign, Channel& channel, Message& )
{
	(void)pos;

	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	InviteChanMode::onShowChanModeEvent(void)
{
}

void	InviteChanMode::onDelete(void* )
{
}

std::string InviteChanMode::toString(void* )
{
	return "";
}
