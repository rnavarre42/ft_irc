#include "Numeric.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::map<int, std::string>	Numeric::_numericMap;
Numeric						*Numeric::_instance;

std::string	Numeric::_toString()
{
	std::ostringstream	ss;

	/*
	 *	Si esta registrado se usa el nick!ident@host,
	 *	si no se usa la del servidor que procesa el mensaje
	 *
	 *	Podemos tener un getMask pure virtual de ISender
	 *	implementada en user o server para esto
	*/

	if (!Numeric::_instance->_server->getName().empty())
		ss << ":" << Numeric::_instance->_server->getName();
	ss << " " << std::setfill('0') << std::setw(3) << this->_num;
	if (Numeric::_instance->_sender->getName().empty())
		ss << " * ";
	else
		ss << " " << Numeric::_instance->_sender->getName() << " ";
	ss << Numeric::_instance->_numericStr;
	return ss.str();
}

void	Numeric::clear(void)
{
	Numeric::_instance->_fieldVector.clear();
}

void	Numeric::insertField(size_t number)
{
	std::ostringstream	oss;

   	oss << number;
	Numeric::insertField(oss.str());
}

void	Numeric::insertField(std::string data)
{
	if (!Numeric::_instance)
		Numeric::_instance = new Numeric();
	Numeric::_instance->_fieldVector.push_back(data);
}

void	Numeric::insertField(char chr)
{
	Numeric::insertField(std::string(1, chr));
}

void	Numeric::_initInstance(Server& server, ISender& sender, int num)
{
	if (!Numeric::_instance)
		Numeric::_instance = new Numeric();
	Numeric::_instance->_server = &server;
	Numeric::_instance->_sender = &sender;
	Numeric::_instance->_num = num;
	Numeric::_instance->_numericStr = Numeric::_numericMap[num];
}

inline bool	isLineFull(std::size_t lineSize)
{
	return  lineSize > MAXLINE - 2;
}

bool	Numeric::_buildMulti(std::string& data, std::string& base, size_t
offset, stringVector_iterator& initialPos)
{
	std::string				line;
	size_t					replacePos;
	stringVector_iterator	current;

	replacePos = base.find('%', offset);
	if (replacePos != std::string::npos)
	{
		base.erase(replacePos, 1);
		if (initialPos == Numeric::_instance->_fieldVector.end())
			return false;
		line = base;
		for (current = initialPos
				; current != Numeric::_instance->_fieldVector.end()
				; ++current)
		{
			if (isLineFull(line.size() + current->size()))
			{
				data.append(line);
				line = base;
			}
			if (line == base && current != initialPos)
				data.append("\r\n");
			if (current != initialPos)
				line.insert(replacePos, " ");
			line.insert(replacePos, *current);
		}
		if (line != base)
			data.append(line);
		return true;
	}
	return false;
}

std::string	Numeric::builder(Server& server, ISender& sender, int num)
{
	size_t					replacePos, offset = 0;
	stringVector_iterator	current;
	std::string				base, data;


	Numeric::_initInstance(server, sender, num);
	for (current = Numeric::_instance->_fieldVector.begin()
			; current != Numeric::_instance->_fieldVector.end()
			; ++current)
	{
		replacePos = Numeric::_instance->_numericStr.find('$', offset);
		if (replacePos == std::string::npos)
			break;
		Numeric::_instance->_numericStr.replace(replacePos, 1, *current);
		offset = replacePos + current->size();
	}
	base = Numeric::_instance->_toString();
	offset += base.size() - Numeric::_instance->_numericStr.size();
	if (!Numeric::_buildMulti(data, base, offset, current))
		data = base;
	Numeric::_instance->_fieldVector.clear();
	return data;
}

Numeric::Numeric(void)
{
	Numeric::_numericMap[ERR_NOTIMPLEMENTED]	= "$ :Command not implemented yet";
	Numeric::_numericMap[ERR_UNKNOWNCOMMAND]	= "$ :Unknown command";
	Numeric::_numericMap[ERR_NICKNAMEINUSE]		= "$ :Is already in use";
	Numeric::_numericMap[ERR_NOTREGISTERED]		= "$ :You have not registered";
	Numeric::_numericMap[ERR_NEEDMOREPARAMS]	= "$ :Not enough parameters";
	Numeric::_numericMap[ERR_ALREADYREGISTERED]	= ":Unauthorized command (already registered)";
	Numeric::_numericMap[ERR_NOPRIVILEGES]		= ":Permision Denied- You're not an IRC operator";
	Numeric::_numericMap[ERR_NOTEXTTOSEND]		= ":No text to send";
	Numeric::_numericMap[ERR_NOSUCHNICK]		= "$ :No such nick/channel";
	Numeric::_numericMap[RPL_MOTDSTART]			= ":- $ Message of the day - ";
	Numeric::_numericMap[RPL_MOTD]				= ":- $";
	Numeric::_numericMap[RPL_ENDOFMOTD]			= ":End of MOTD command";
	Numeric::_numericMap[ERR_NOMOTD]			= ":MOTD File is missing";
	Numeric::_numericMap[RPL_UNAWAY]			= ":You are no longer marked as being away";
	Numeric::_numericMap[RPL_NOAWAY]			= ":You have been marked as being away";
	Numeric::_numericMap[ERR_USERONCHANNEL]		= "$ $ :is already on channel";
	Numeric::_numericMap[ERR_CHANNELISFULL]		= "$ :Cannot join channel (+l)";
	Numeric::_numericMap[ERR_INVITEONLYCHAN]	= "$ :Cannot join channel (+i)";
	Numeric::_numericMap[ERR_BANNEDFROMCHAN]	= "$ :Cannot join channel (+b)";
	Numeric::_numericMap[ERR_BADCHANNELKEY]		= "$ :Cannot join channel (+k)";
	Numeric::_numericMap[ERR_BADCHANMASK]		= "$ :Bad channel Mask";
	Numeric::_numericMap[ERR_TOOMANYCHANNELS]	= "$ :You have joined too many channels";
	Numeric::_numericMap[ERR_NOSUCHCHANNEL]		= "$ :No such channel";
	Numeric::_numericMap[ERR_USERNOTINCHANNEL]	= "$ :They aren't on that channel";
	Numeric::_numericMap[ERR_NOTONCHANNEL]		= "$ :You're not on that channel";
	Numeric::_numericMap[RPL_WELCOME]			= ":Welcome to the Internet Relay Network $";
	Numeric::_numericMap[RPL_YOURHOST]			= ":Your host is $, running version " VERSION;
	Numeric::_numericMap[RPL_CREATED]			= ":This server was created " __TIME__ " " __DATE__;
	Numeric::_numericMap[RPL_MYINFO]			= "$ " VERSION " $ $ :$";
	Numeric::_numericMap[RPL_ISUPPORT]			= "% :are supported by this server";
	Numeric::_numericMap[ERR_ERRONEUSNICKNAME]	= "$ :Erroneus nickname";
	Numeric::_numericMap[RPL_NAMREPLY]			= "$ $ :%";
	Numeric::_numericMap[RPL_ENDOFNAMES]		= "$ :End of NAMES list";
	Numeric::_numericMap[RPL_INVITING]			= "$ :$";
	Numeric::_numericMap[ERR_CHANOPRIVSNEEDED]	= "$ :You're not channel operator";
	Numeric::_numericMap[ERR_USERSDONTMATCH]	= ":Cannot change mode for other users";
	Numeric::_numericMap[ERR_UNKNOWNMODE]		= "$ :is unknown mode char to me for $";
	Numeric::_numericMap[RPL_TOPIC]				= "$ :$";
	Numeric::_numericMap[RPL_NOTOPIC]			= "$ :No topic is set";
	Numeric::_numericMap[RPL_TOPICTIME]			= "$ :$ $";
	Numeric::_numericMap[ERR_CANNOTSENDTOCHAN]	= "$ :Cannot send to channel";
	Numeric::_numericMap[ERR_CANTCHANGENICK]	= ":Cannot change nickname while on $ (you're banned)";
	Numeric::_numericMap[RPL_LIST]				= "$ $ :$";
	Numeric::_numericMap[RPL_LISTEND]			= ":End of LIST";
	Numeric::_numericMap[ERR_PASSWDMISMATCH]	= ":Password incorrect";
	Numeric::_numericMap[RPL_YOUREOPER]			= ":You are now an IRC operator";
	Numeric::_numericMap[ERR_NOOPERHOST]		= "No O-lines for your host";
	Numeric::_numericMap[RPL_ENDOFWHOIS]		= "$ :End of WHOIS list";
	Numeric::_numericMap[RPL_WHOISUSER]			= "$ $ $ * :$";
	Numeric::_numericMap[RPL_WHOISHOST]			= "$ :is connection from $ $";
	Numeric::_numericMap[RPL_WHOISCHANNELS]		= "$ :%";
	Numeric::_numericMap[RPL_WHOISSERVER]		= "$ $ :$";
	Numeric::_numericMap[RPL_WHOISIDLE]			= "$ $ $ :seconds idle, signon time";
	Numeric::_numericMap[RPL_WHOISOPERATOR]		= "$ :is an IRC operator";
	Numeric::_numericMap[RPL_WHOISMODES]		= "$ :is using modes +$";
	Numeric::_numericMap[RPL_VISIBLEHOST]		= "$ :is now your visible host";
	Numeric::_numericMap[RPL_CHANNELMODEIS]		= "$ :$ %";
	Numeric::_numericMap[RPL_CREATIONTIME]		= "$ :$";
	Numeric::_numericMap[RPL_EXCEPTLIST]		= "$ $";
	Numeric::_numericMap[RPL_ENDOFEXCEPTLIST]	= "$ :End of channel exception list";
	Numeric::_numericMap[RPL_BANLIST]			= "$ $";
	Numeric::_numericMap[RPL_ENDOFBANLIST]		= "$ :End of channel ban list";
	Numeric::_numericMap[RPL_UMODEIS]			= ":$";
	Numeric::_numericMap[RPL_AWAY]				= "$ :$";
}
