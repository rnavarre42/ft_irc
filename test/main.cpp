#include "Server.hpp"
#include <string>

int main(void)
{
	Server				server("irc.42born2code.com");

	server.start();
	server.doEvents();
}
