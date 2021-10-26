#include "Console.hpp"
#include <unistd.h>
#include <string>

Console::Console(Server server) : server(server)
{}

size_t	Console::write(std::string msg)
{
	return (::write(0, msg.c_str(), msg.length()));
}
