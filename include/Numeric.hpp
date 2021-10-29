#pragma once

#include "Message.hpp"
#include <map>
#include <string>

class Numeric
{
public:
	~Numeric(void);
	static std::string	builder(Message &msg, int num, std::string p[] = 0, size_t size = 0);
	
private:
	Numeric(void);
	std::string	_toString(Message &msg, int num, std::string str);

	static std::map<int, std::string>	_numericMap;
};
