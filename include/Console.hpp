#ifndef CONSOLE_HPP
# define CONSOLE_HPP

# include "log.hpp"
# include "Server.hpp"
# include <string>

class Console
{
public:
	Console(Server &server);
	~Console(void);
		
	static void	log(int logLevel, std::string str);

private:
	Console(Console const &src);
	Console &operator=(Console const &rhs);

	Server	&_server;
};

#endif
