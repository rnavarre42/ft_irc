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
//	ss << _instance->message->getCmd() << _instance->numericStr;
	ss << Numeric::_instance->_numericStr;
//	std::cout << msg.getSender().getName() << " " << num << " * " << msg.getCmd() << " " << str << std::endl;

	return ss.str();
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

std::string	Numeric::builder(Server &server, ISender &sender, int num)
{
	size_t					replacePos, offset = 0;
	stringVector_iterator	it1, it2;
	std::string				base, line, data;

	if (!Numeric::_instance)
		_instance = new Numeric();
	Numeric::_instance->_server = &server;
	Numeric::_instance->_sender = &sender;
	Numeric::_instance->_num = num;
	Numeric::_instance->_numericStr = Numeric::_numericMap[num];
	for (it1 = _instance->_fieldVector.begin(); it1 != _instance->_fieldVector.end(); it1++)
	{
		replacePos = Numeric::_instance->_numericStr.find('$', offset);
		if (replacePos == std::string::npos)
			break;
		Numeric::_instance->_numericStr.replace(replacePos, 1, *it1);
		offset = replacePos + it1->size();
	}
	base = Numeric::_instance->_toString();
	offset += base.size() - Numeric::_instance->_numericStr.size();
	if ((replacePos = base.find('%', offset)) != std::string::npos)
	{
		base.erase(replacePos, 1);
		line = base;
		for (it2 = it1; it2 != _instance->_fieldVector.end(); it2++)
		{
//			std::cout << "line =" << line << " offset = " << offset <<  std::endl;
//			std::cout << "names = " << *it2 << std::endl;
			if ((line.size() + it2->size()) > 52 - 2 )// si hemos llenado
			{
				data.append(line);
				line = base;
			}
			if (line == base && it1 != it2)
				data.append("\r\n");
			if (it1 != it2)
				line.insert(replacePos, " ");
			line.insert(replacePos, *it2);
		}
		if (line != base)
			data.append(line);
	}
	else
		data = base;
	Numeric::_instance->_fieldVector.clear();
	return data;
}

Numeric::Numeric(void)
{
	Numeric::_numericMap[ERR_NOTIMPLEMENTED] = "$ :Command not implemented yet";
	Numeric::_numericMap[ERR_UNKNOWNCOMMAND] = "$ :Unknown command";
	Numeric::_numericMap[ERR_NICKNAMEINUSE] = "$ :Is already in use";
	Numeric::_numericMap[ERR_NOTREGISTERED] = "$ :You have not registered";
	Numeric::_numericMap[ERR_NEEDMOREPARAMS] = "$ :Not enough parameters";
	Numeric::_numericMap[ERR_ALREADYREGISTERED] = ":Unauthorized command (already registered)";
	Numeric::_numericMap[ERR_NOPRIVILEGES] = ":Permision Denied- You're not an IRC operator";
	Numeric::_numericMap[ERR_NOTEXTTOSEND] = ":No text to send";
	Numeric::_numericMap[ERR_NOSUCHNICK] = "$ :No such nick/channel";
	Numeric::_numericMap[RPL_MOTDSTART] = ":- $ Message of the day - ";
	Numeric::_numericMap[RPL_MOTD] = ":- $";
	Numeric::_numericMap[RPL_ENDOFMOTD] = ":End of MOTD command";
	Numeric::_numericMap[ERR_NOMOTD] = ":MOTD File is missing";
	Numeric::_numericMap[RPL_UNAWAY] = ":You are no longer marked as being away";
	Numeric::_numericMap[RPL_NOAWAY] = ":You have been marked as being away";
	Numeric::_numericMap[ERR_USERONCHANNEL] = "$ $ :is already on channel";
	Numeric::_numericMap[ERR_CHANNELISFULL] = "$ :Cannot join channel (+l)";
	Numeric::_numericMap[ERR_INVITEONLYCHAN] = "$ :Cannot join channel (+i)";
	Numeric::_numericMap[ERR_BANNEDFROMCHAN] = "$ :Cannot join channel (+b)";
	Numeric::_numericMap[ERR_BADCHANNELKEY] = "$ :Cannot join channel (+k)";
	Numeric::_numericMap[ERR_BADCHANMASK] = "$ :Bad channel Mask";
	Numeric::_numericMap[ERR_TOOMANYCHANNELS] = "$ :You have joined too many channels";
	Numeric::_numericMap[ERR_NOSUCHCHANNEL]	= "$ :No such channel";
	Numeric::_numericMap[ERR_USERNOTINCHANNEL]	= "$ :They aren't on that channel";
	Numeric::_numericMap[ERR_NOTONCHANNEL]	= "$ :You're not on that channel";
	Numeric::_numericMap[RPL_WELCOME] =  "Welcome to the Internet Relay Network $";
	Numeric::_numericMap[RPL_YOURHOST] = "Your host is $, running version " VERSION;
	Numeric::_numericMap[RPL_CREATED] = "This server was created " __TIME__ " " __DATE__;
	Numeric::_numericMap[RPL_MYINFO] = "$ " VERSION " $ $";
	Numeric::_numericMap[RPL_ISUPPORT] = "$ :are supported by this server";
	Numeric::_numericMap[ERR_ERRONEUSNICKNAME] = "$ :Erroneus nickname";
	Numeric::_numericMap[RPL_NAMREPLY] = "$ $ :%";
	Numeric::_numericMap[RPL_ENDOFNAMES] = "$ :End of NAMES list";
	Numeric::_numericMap[RPL_INVITING] = "$ :$";
	Numeric::_numericMap[ERR_CHANOPRIVSNEEDED]	= "$ :You're not channel operator";
	Numeric::_numericMap[ERR_USERSDONTMATCH]	= ":Cannot change mode for other users";
	Numeric::_numericMap[ERR_UNKNOWNMODE] = "$ :is unknown mode char to me for $";
}
