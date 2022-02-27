#include <string>

std::string strToUpper(std::string data)
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
