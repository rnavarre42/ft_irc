#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bouncer.h"

t_client	*accept_client(t_server *server)
{
	t_client	*client;
	int			newfd;

	client = get_client_slot(server);
	newfd = accept(server->fd, (struct sockaddr *)&server->address, (socklen_t *)&server->addrlen);
	if (newfd < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	if (!client)
	{
		send(newfd, "The server is full. Please, try again more later.\n\r", 51, 0);
		close(newfd);
		return (NULL);
	}
	client->fd = newfd;
	client->poll_index = get_poll_slot(server);
//	FD_SET(client->fd, &server->set);
	server->pollfds[client->poll_index].fd = client->fd;
	server->pollfds[client->poll_index].events = POLLIN; 
	return (client);
}
