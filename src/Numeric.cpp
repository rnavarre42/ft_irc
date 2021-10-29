#include "Numeric.hpp"
#include "Message.hpp"
#include <iostream>
#include <sstream>
#include <string>

std::map<int, std::string> Numeric::_numericMap;

std::string	Numeric::_toString(Message &msg, int num, std::string str)
{
	std::ostringstream	ss;

	ss << msg.getSender().getName();
	ss << " " << num << " * ";
	ss << msg.getCmd() << " " << str;
	return ss.str();
}

std::string	Numeric::builder(Message &msg, int num, std::string p[], size_t size)
{
	static Numeric	*instance;

	size_t		i = 0;
	std::string str;
	size_t		replacePos, offset = 0;

	if (!instance)
		instance = new Numeric();
	str = Numeric::_numericMap[num];
	std::cout << "num = " << num << " : str = " << str << std::endl;
	while (i < size)
	{
		replacePos = str.find('$', offset);
		str.replace(replacePos, 1, p[i]);
		offset += replacePos + p[i].size();
		i++;
	}
	return instance->_toString(msg, num, str);
}

Numeric::Numeric(void)
{
	Numeric::_numericMap[421] = "$ :Unknown command";
	Numeric::_numericMap[433] = "$ :Is already in use";
	Numeric::_numericMap[451] = ":You have not registered";
	Numeric::_numericMap[461] = "$ :Not enough parameters";
	Numeric::_numericMap[462] = ":Unauthorized command (already registered)";
}
