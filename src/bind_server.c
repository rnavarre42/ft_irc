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
	FD_ZERO(&server->set);
	FD_SET(server->fd, &server->set);
	FD_SET(0, &server->set);
}
