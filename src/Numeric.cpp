#include "Numeric.hpp"
#include "Message.hpp"
#include "numerics.hpp"
#include <iostream>
#include <sstream>
#include <string>

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

	if (!_instance->server.getName().empty())
		ss << ":" << _instance->server.getName();
	ss << " " << num;
	if (_instance->message->getSender().getName().empty())
		ss << " * ";
	else
		ss << " " << _instance->message->getSender().getName();
	ss << _instance->message->getCmd() << _instance->numericStr;

//	std::cout << msg.getSender().getName() << " " << num << " * " << msg.getCmd() << " " << str << std::endl;

	return ss.str();
}

std::string	Numeric::builder(Server &server, Message &message, int num, std::string p[], size_t size)
{
	size_t		i = 0;
	size_t		replacePos, offset = 0;

	if (!Numeric::_instance)
		_instance = new Numeric(server);
	_instance->message = &message;
	_instance->num = num;
	_instance->numericStr = Numeric::_numericMap[num];
	std::cout << "num = " << num << " : str = " << _instance->numericStr << std::endl;
	while (i < size)
	{
		replacePos = _instance->numericStr.find('$', offset);
		_instance->numericStr.replace(replacePos, 1, p[i]);
		offset += replacePos + p[i].size();
		i++;
	}
	return _instance->_toString();
}

Numeric::Numeric(Server &server) : server(server)
{
	Numeric::_numericMap[ERR_UNKNOWNCOMMAND] = "$ :Unknown command";
	Numeric::_numericMap[ERR_NICKNAMEINUSE] = "$ :Is already in use";
	Numeric::_numericMap[ERR_NOTREGISTERED] = " :You have not registered";
	Numeric::_numericMap[ERR_NEEDMOREPARAMS] = "$ :Not enough parameters";
	Numeric::_numericMap[ERR_ALREADYREGISTERED] = " :Unauthorized command (already registered)";
}
