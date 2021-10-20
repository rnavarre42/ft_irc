#include <stdio.h>
#include <stdlib.h>
#include "bouncer.h"

void		bind_server(t_server *server)
{
	server->address.sin_family = AF_INET;
	server->address.sin_addr.s_addr = INADDR_ANY;
	server->address.sin_port = htons(server->port);
	if (bind(server->fd, (struct sockaddr *)&server->address,
			sizeof(server->address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server->fd, 3) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	server->pollfds[0].fd = 0;
	server->pollfds[0].events = POLLIN; 
	server->pollfds[1].fd = server->fd;
	server->pollfds[1].events = POLLIN; 
}
