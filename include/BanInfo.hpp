#ifndef BANINFO_HPP
# define BANINFO_HPP

# include "utils.hpp"

# include <string>
# include <ctime>

struct BanInfo
{
	BanInfo(std::string mask, std::string nick)
		: mask(mask)
		, nick(nick)
		, time(std::time(NULL))
	{}

	std::string	mask;
	std::string	nick;
	time_t		time;

	bool	operator==(const std::string& rhs)
	{
		return wildcardComp(this->mask.begin(), rhs.begin());
	}
};

#endif
