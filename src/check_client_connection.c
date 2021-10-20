#include <time.h>
#include <stdio.h>
#include "bouncer.h"

void	print_client(t_server *server, t_client *client)
{
	printf("client->id %lu\n", client->id);
	printf("client->fd %i\n", client->fd);
	printf("client->poll_index %i\n", client->poll_index);
	printf("pollfds->fd = %i\n", server->pollfds->fd);
}

void	print_pollfds(t_server *server)
{
	for (int i = 0; i < MAXCLIENTS + 2; i++)
	{
		printf("pollfds[%i].fd = %i\n", i, server->pollfds[i].fd);
	}
}

int	check_client_connection(t_server *server)
{
	t_client	*client;

	if (server->pollfds[1].revents & POLLIN)//FD_ISSET(server->fd, &server->cset))
	{
		client = accept_client(server);
		if (client)
		{
//			print_client(server, client);
//			print_pollfds(server);
			client->logon_time = time(NULL);
			client->id = server->last_id++;
			server->clients_count++;
			printf("New connection. Hello message sent\n");
			send_except(server, client, "New user #%i connected.\n", client->id);
			send_to(client, HELLO_STRING);
			send_to(client, "You are the connection #%i width fd %i in this server.\n\rYour logon time is %lu\n\r", client->id, client->fd, client->logon_time);
			send_to(client, "To disconnect from server, press ctrl+c\n");
		}
		return (1);
	}
	else
		return (0);
}
