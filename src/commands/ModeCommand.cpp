#include "ModeCommand.hpp"
#include "AChanMode.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Numeric.hpp"
#include "Console.hpp"
#include "ChanModeConfig.hpp"

#include <iostream>

ModeCommand::ModeCommand(Server& server, int accessLevel, int paramCount)
	: ACommand(server, accessLevel, paramCount)
{}

ModeCommand::~ModeCommand(void)
{}

void	ModeCommand::loadEvents(Server::eventHandler_type&)
{}

void	ModeCommand::unloadEvents(Server::eventHandler_type&)
{}

inline int	isSignMode(char c)
{
	return (c == '+') * 1 + (c == '-') * 2;
}

inline bool	hasParamMode(bool set, AChanMode* chanMode)
{
	return ((set && chanMode->getConfig().type & ChanModeConfig::enableParam)
			|| (!set && chanMode->getConfig().type & ChanModeConfig::disableParam));
}

void	cleanSignModes(std::string& modes)
{
	int	set = 3;

	for (std::string::iterator it = modes.begin(); it != modes.end();)
	{
		if ((isSignMode(*it) && ((1 + it) == modes.end() || isSignMode(*(1 + it))))
				|| isSignMode(*it) == set)
			modes.erase(it);
		else
		{
			if (isSignMode(*it))
				set = isSignMode(*it);
			++it;
		}
	}
}

/*
 *	Funcion que verifica si el modo de canal existe, la sintaxis es correcta y tiene privilegios para
 *	ejecutarlo.
 */

void	ModeCommand::_checkChanModes(Message& message)
{
	unsigned long					pos = 2;
	bool							set = true;
	std::string&					target = message[0];
	std::string&					modes = message[1];
	Channel*						channel;
	AChanMode*						chanMode;
	std::string::iterator 			currentIt;
	size_t							initialSize;

	if (!(channel = server.channelAt(target)))
	{
		Numeric::insertField(target);
		message.replyNumeric(ERR_NOSUCHCHANNEL);
		return ;
	}
	if (message.size() < 2)
	{
		Numeric::insertField(target);
		this->server.chanModeNames(*channel);
		message.replyNumeric(RPL_CHANNELMODEIS);
		Numeric::insertField(target);
		Numeric::insertField(to_string(channel->getCreationTime()));
		message.replyNumeric(RPL_CREATIONTIME);
		return ;
	}
	initialSize = modes.size();
	message.setChannel(channel);
	for (std::string::iterator strIt = modes.begin(); strIt != modes.end(); )
	{
		currentIt = strIt;
		++strIt;
		if (*currentIt == '+')
			set = true;
		else if (*currentIt == '-')
			set = false;
		else if ((chanMode = server.findChanMode(*currentIt)))
		{
			if (!chanMode->getConfig().unique && initialSize == 1 && message.size() <= pos)
			{
				chanMode->onShowChanModeEvent(*channel, message);
				return ;
			}
			else if (hasParamMode(set, chanMode) && message.size() <= pos)
			{
				Numeric::insertField(message.getCmd());
				message.replyNumeric(ERR_NEEDMOREPARAMS);
				modes.erase(currentIt);
				--strIt;
			}
			else if (!server.checkChannelMode(message, COMMAND_MODE)) // Si el usuario no es operador...
			{
				modes.erase(currentIt);
				--strIt;
				if (hasParamMode(set, chanMode) && message.size() > pos)
					message.eraseAt(pos);
			}
			else
			{
				if (message.size() >= pos && chanMode->onChanModeEvent(pos, set, *channel, message))
				{
					if (hasParamMode(set, chanMode) && message.size() > pos)
						++pos;
				}
				else
				{
					modes.erase(currentIt);
					--strIt;
					if (hasParamMode(set, chanMode) && message.size() > pos)
						message.eraseAt(pos);
				}
			}
		}	
		else
		{
			Numeric::insertField(*currentIt);
			Numeric::insertField(channel->getName());
			message.replyNumeric(ERR_UNKNOWNMODE);
			modes.erase(currentIt);
			--strIt;
		}
	}
	cleanSignModes(modes);
	message.limitMaxParam(pos);
	if (!modes.empty())
	{
		message.setReceiver(channel);
		message.setReceiver(this->userSender);
		message.hideReceiver();
		message.hideSemicolon();
		message.send();
	}
}

/*
 *	Funcion que verifica si el modo de usuario existe, la sintaxis es correcta y tiene privilegios para
 *	ejecutarlo.
 */ 

void	ModeCommand::_checkUserModes(Message& message)
{
	std::string&	modeUserName = message[0];
	std::string&	modes = message[1];
	User*			modeUser = server.userAt(modeUserName);

	if (!(modeUser = server.userAt(modeUserName)))
	{
		Numeric::insertField(modeUserName);
		message.replyNumeric(ERR_NOSUCHNICK);
		return ;
	}	  
	if (*this->userSender != *modeUser)
	{
		Numeric::insertField(modeUser->getName());
		message.replyNumeric(ERR_USERSDONTMATCH);
		return ;
	}
	if (modes == "-o" && modeUser->isOper())
	{
		modeUser->setOper(false);
		modeUserName = modeUser->getName();
		message.reply();
	}
}

bool	ModeCommand::_recvUser(Message& message)
{
	std::string&	channelName = message[0];

	if (server.isChannel((channelName)))
		this->_checkChanModes(message);
	else
		this->_checkUserModes(message);
	return true;
}

bool	ModeCommand::_recvServer(Message&)
{
	return false;
}

bool	ModeCommand::_sendUser(Message&)
{
	return false;
}

bool	ModeCommand::_sendServer(Message&)
{
	return false;
}
