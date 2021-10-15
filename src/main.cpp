#include <iostream>
#include <string>
#include "Server.hpp"
/*
_Bool	telnet_ctrlc(char *buffer)
{
	char exit[] = {255, 244, 255, 253, 6};

	return (!strncmp(buffer, exit, 5));
}
*/
int main(void)
{
	Server	server = Server::getInstance("0.0.0.0", 6667);

	std::cout << "Waiting connect clients..." << std::endl;
	server.start();
}
