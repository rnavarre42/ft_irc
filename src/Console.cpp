#include "Console.hpp"
#include "log.hpp"
#include "Server.hpp"
#include <iostream>
#include <iomanip>
#include <string>

Console::Console(Server server) : server(server)
{}

void	Console::log(int logLevel, std::string msg)
{
	time_t			timestamp = time(0);
	struct std::tm	*ltm = std::localtime(&timestamp);
	int				bits = 1;
	std::string		logEvent[] = {"DEFAULT", "\033[32mINFO\033[0m", "\033[33mWARNING\033[0m", "\033[31mERROR\033[0m", "\033[35mFATAL\033[0m"};
	for (int i = logLevel; i > 1; i /= 2)
		bits++;	
	if (LOG_LEVEL & logLevel)
	{
		std::cerr << std::setfill('0') << "[" << 
			std::setw(2) << ltm->tm_hour << "." <<
			std::setw(2) << ltm->tm_min << "." <<
			std::setw(2) << ltm->tm_sec << " " <<
			logEvent[(bits)] << "] " << msg << std::endl;
	}
//	return (::write(0, (msg.c_str(), msg.length()));
}
