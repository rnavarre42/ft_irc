#ifndef CONSOLE_HPP
# define CONSOLE_HPP

# include "log.hpp"
# include "Server.hpp"
# include <string>

class Console
{
public:
	Console(Server server);
		
	static void	log(int logLevel, std::string str);

private:
	Server	_server;
};

#endif
