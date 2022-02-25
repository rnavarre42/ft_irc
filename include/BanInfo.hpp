#ifndef BANINFO_HPP
# define BANINFO_HPP

# include <string>
# include <ctime>

struct BanInfo
{
	BanInfo(std::string mask, std::string nick)
		: mask(mask), nick(nick), time(std::time(NULL))
	{}

	std::string	mask;
	std::string	nick;
	time_t		time;
/*
	bool	operator==(const std::string& rhs)
	{
		std::string::iterator	 = mask.begin();
		std::string::iterator	rhsIt = rhs.begin();

		return false;
	}
*/
};

#endif
