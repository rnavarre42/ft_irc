#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>

std::string strToUpper(std::string data);
bool		wildcardComp(std::string::const_iterator pattern, std::string::const_iterator str);
bool		isValidNick(const std::string& value);
bool		isSpecialLetter(const char chr);
bool		isValidChanName(const std::string& value);

#endif
