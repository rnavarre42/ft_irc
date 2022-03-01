#include "Server.hpp"
#include "utils.hpp"

#include <string>

std::string	strToUpper(std::string data)
{
	for (size_t i = 0; i < data.size(); i++)
		if (data[i] >= 'a' && data[i] <= '~')
			data[i] -= 32;
	return data;
}

bool	wildcardComp(std::string::const_iterator pattern, std::string::const_iterator str)
{
	if (*pattern == '\0')
		return *str == '\0';
	if (*pattern == '?' || *pattern == *str)
		return *str != '\0' && wildcardComp(1 + pattern, 1 + str);
	if (*pattern == '*')
		return wildcardComp(1 + pattern, str)
			|| (*str != '\0' && wildcardComp(pattern, 1 + str));
	return false;
}

std::string	addWildcard(std::string value)
{
	if (value.find('!') != std::string::npos)
	{
		if (value.find('@') != std::string::npos)
			return value;
		else
			return value + "@*";
	}
	if (value.find('@') != std::string::npos)
		return "*!" + value;
	return value + "!*@*";
}

bool	isSpecialLetter(const char chr)
{
	return chr >= 'A' && chr <= '~';
}

bool	isValidNick(const std::string& value)
{
	for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
	{
		if (!std::isdigit(*it) && !isSpecialLetter(*it) && *it != '-')
			return false;
	}
	return true;
}

inline bool	isChanNameSymbol(const unsigned char chr)
{
	return (chr >= 0x01 && chr <= 0x09)
		|| (chr >= 0x0B && chr <= 0x0C)
		|| (chr >= 0x0E && chr <= 0x1F)
		|| (chr >= 0x21 && chr <= 0x2B)
		|| (chr >= 0x2D && chr <= 0x39)
		|| (chr >= 0x3B && chr <= 0xFF);
}

bool	isValidChanName(const std::string& value)
{
	if (value.size() > CHANNELLEN)
		return false;
	for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
	{
		if (!isChanNameSymbol(*it))
			return false;
	}
	return true;
}
