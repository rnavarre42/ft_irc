#pragma once

#include <map>
#include <string>

class Numeric
{
public:
	~Numeric(void);
	static std::string	builder(int num, std::string p[] = 0, size_t size = 0);
	
private:
	Numeric(void);
	static std::map<int, std::string>	_numericMap;
};
