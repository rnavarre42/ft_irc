#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>

std::string strToUpper(std::string data);
bool		wildcardComp(std::string::const_iterator pattern, std::string::const_iterator str);

#endif
