#include "Server.hpp"
#include <iostream>
#include <string>
#include <stdlib.h>

/*
_Bool	telnet_ctrlc(char *buffer)
{
	char exit[] = {255, 244, 255, 253, 6};

	return !strncmp(buffer, exit, 5);
}
*/

void leaks(void)
{
#ifdef __APPLE__
	system("leaks ircserv");
#endif
}

int main(void)
{
	Server&	server = Server::createInstance("0.0.0.0", 6667, "irc.castela.es");

	atexit(leaks);
	server.run();
	Server::deleteInstance();;
	return 0;
}
