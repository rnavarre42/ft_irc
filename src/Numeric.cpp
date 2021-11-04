#include "Numeric.hpp"
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

	if (!_instance->server->getName().empty())
		ss << ":" << _instance->server->getName();
	ss << " " << num;
	if (_instance->sender->getName().empty())
		ss << " * ";
	else
		ss << " " << _instance->sender->getName() << " ";
//	ss << _instance->message->getCmd() << _instance->numericStr;
	ss << _instance->numericStr;
//	std::cout << msg.getSender().getName() << " " << num << " * " << msg.getCmd() << " " << str << std::endl;

	return ss.str();
}

void	Numeric::insertField(std::string data)
{
	if (!Numeric::_instance)
		_instance = new Numeric();
	_instance->fieldVector.push_back(data);
}

std::string	Numeric::builder(Server &server, ISender &sender, int num)
{
	size_t		replacePos, offset = 0;

	if (!Numeric::_instance)
		_instance = new Numeric();
	_instance->server = &server;
	_instance->sender = &sender;
	_instance->num = num;
	_instance->numericStr = Numeric::_numericMap[num];
	for (size_t i = 0; i < _instance->fieldVector.size(); i++)
	{
		replacePos = _instance->numericStr.find('$', offset);
		_instance->numericStr.replace(replacePos, 1, _instance->fieldVector[i]);
		offset += replacePos + _instance->fieldVector[i].size();
	}
	_instance->fieldVector.clear();
	return _instance->_toString();
}

Numeric::Numeric(void)
{
	Numeric::_numericMap[ERR_NOTIMPLEMENTED] = "$ :Command not implemented yet";
	Numeric::_numericMap[ERR_UNKNOWNCOMMAND] = "$ :Unknown command";
	Numeric::_numericMap[ERR_NICKNAMEINUSE] = "$ :Is already in use";
	Numeric::_numericMap[ERR_NOTREGISTERED] = ":You have not registered";
	Numeric::_numericMap[ERR_NEEDMOREPARAMS] = "$ :Not enough parameters";
	Numeric::_numericMap[ERR_ALREADYREGISTERED] = ":Unauthorized command (already registered)";
	Numeric::_numericMap[ERR_NOPRIVILEGES] = ":Permision Denied- You're not an IRC operator";
	Numeric::_numericMap[ERR_NOTEXTTOSEND] = ":No text to send";
	Numeric::_numericMap[ERR_NOSUCHNICK] = "$ :No such nick/channel";
	Numeric::_numericMap[ERR_BADCHANMASK] = "$ :Bad channel Mask";
}
