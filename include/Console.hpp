#ifndef CONSOLE_HPP
# define CONSOLE_HPP

# include <string>
# include "Server.hpp"

class Console
{
public:
	Console(Server server);
		
	static void	log(int logLevel, std::string cmd);

private:
	Server	server;
};

#endif
