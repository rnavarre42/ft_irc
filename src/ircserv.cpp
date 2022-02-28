#include "Server.hpp"
#include <iostream>
#include <string>
#include <stdlib.h>

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

	if (argc != 3)
	{
		std::cerr << "Usage:" << std::endl
				  << "./" << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}	
	serverPort = atoi(argv[1]);
	if (!serverPort && serverPort >= 65536)
	{
		std::cerr << "Error: Invalid port number" << std::endl;
		return 1;
	}

	server = Server::createInstance("0.0.0.0", serverPort, "irc.castela.es", argv[2]);

	atexit(leaks);
	server->run();
	Server::deleteInstance();;
	return 0;
}
