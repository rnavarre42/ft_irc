#include "Client.hpp"
#include <iostream>

int	main(int argc, char **argv)
{
	if (argc != 5)
	{
		std::cerr << "Usage: ./irclient host port nick ident" << std::endl;
		return 1;
	}

	Client*	client = Client::createInstance(argv[1], argv[2], argv[3], argv[4]);

	client->start();

	Client::deleteInstance();
	return 0;
}
