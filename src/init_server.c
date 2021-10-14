#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bouncer.h"

t_server	*init_server(char *ip, int port, int timeout)
{
	t_server	*server;

	server = singleton_server();
	if (!server)
		return (NULL);
	bzero(server, sizeof(t_server));
	server->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (!server->fd)
	{
		perror("Create socket failed");
		exit(EXIT_FAILURE);
	}
	server->opt = 1;
	server->ip = ip;
	server->port = port;
	server->selecttimeout = timeout;
	server->addrlen = sizeof(server->address);
	if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR
			, &server->opt, sizeof(server->opt)))
	//		| SO_REUSEPORT <-- mac compatibility
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	return (server);
}
