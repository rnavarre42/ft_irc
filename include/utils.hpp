#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>

std::string strToUpper(std::string data);

bool		wildcardComp(std::string::const_iterator pattern, std::string::const_iterator str);
std::string	addWildcard(std::string value);

bool	isValidNick(const std::string& value);
bool	isSpecialLetter(const char chr);
bool	isValidChanName(const std::string& value);

#endif
