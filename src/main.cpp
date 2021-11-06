#include "Server.hpp"
#include <iostream>
#include <string>
#include <stdlib.h>

/*
_Bool	telnet_ctrlc(char *buffer)
{
	char exit[] = {255, 244, 255, 253, 6};

	return (!strncmp(buffer, exit, 5));
}
*/

void leaks(void)
{
//	system("leaks ircserv");
}

int main(void)
{
	Server	&server = Server::getInstance("0.0.0.0", 6667, "irc.42born2code.com");

	atexit(leaks);
	std::cout << "Waiting connect clients..." << std::endl;
	server.start();
	Server::deleteInstance();;
	return 0;
}
