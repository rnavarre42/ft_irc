#ifndef CONSOLE_HPP
# define CONSOLE_HPP

#include <string>
#include "Server.hpp"

class Console
{
	public:
		Console(Server server);
		
		size_t	write(std::string cmd);

	private:
		Server	server;
};

#endif
