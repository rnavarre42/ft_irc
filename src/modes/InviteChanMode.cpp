#include "InviteChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Numeric.hpp"

InviteChanMode::InviteChanMode(Server &server)
	: AChanMode(server)
{
	this->_chanModeConfig.type = ChanModeConfig::noParam;
	this->_chanModeConfig.mode = 'i';
	this->_chanModeConfig.events = CHANMODE_JOIN;
}

InviteChanMode::~InviteChanMode(void)
{}

void	InviteChanMode::onChanEvent(Access &access, Message &message)
{
	Channel	*channel = message.getChannel();

	access = AChanMode::allow;
	if (this->isSetMode(*channel)
			&& (this->_server.invite().find(message.getSender(), channel)) == this->_server.invite().end())
	{
		Numeric::insertField(message.getChannel()->getName());
		message.replyNumeric(ERR_INVITEONLYCHAN);
		access = AChanMode::deny;
	}
}

bool	InviteChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &)
{
	(void)pos;

	return (sign && this->setMode(channel, NULL))
				|| (!sign && this->unsetMode(channel, NULL));
}

void	InviteChanMode::onShowChanModeEvent(void)
{
}

void	InviteChanMode::onDelete(void *)
{
}

std::string InviteChanMode::getValue(void *)
{
	return "";
}
