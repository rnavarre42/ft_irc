#include <stdio.h>
#include <unistd.h>
#include "bouncer.h"

void	disconnect_client(t_server *server, t_client *client)
{
	printf("Client #%lu close connection\n", client->id);
	send_except(server, client, "- #%i Disconnected\n\r", client->id);
//	FD_CLR(client->fd, &server->set);
	close(client->fd);
	server->pollfds[client->poll_index].fd = 0;
	client->fd = 0;
	server->clients_count--;
}
