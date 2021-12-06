#include "Client.hpp"
#include <iostream>

int	main(int argc, char **argv)
{
	if (argc != 5)
	{
		std::cerr << "Usage: client hostname port" << std::endl;
		return 1;
	}

	Client	client(argv[1], argv[2], argv[3], argv[4]);

	return 0;
}
