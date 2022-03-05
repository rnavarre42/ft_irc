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

int main(int argc, char** argv)
{
	Server*			server;
	unsigned int	serverPort;
	std::string		serverPass;

	if (argc != 3)
	{
		std::cerr << "Usage:" << std::endl
				  << "./" << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}
	serverPort = atoi(argv[1]);
	serverPass = argv[2];
	if (!serverPort && serverPort >= 65536)
	{
		std::cerr << "Error: Invalid port number" << std::endl;
		return 1;
	}
	server = Server::createInstance("0.0.0.0", serverPort, serverPass, "irc.castela.es", "Un servidor de lo mas atrevido");

	atexit(leaks);
	server->run();
	Server::deleteInstance();;
	return 0;
}
