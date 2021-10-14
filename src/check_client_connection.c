#include <time.h>
#include <stdio.h>
#include "bouncer.h"

void	check_client_connection(t_server *server)
{
	t_client	*client;

	if (FD_ISSET(server->fd, &server->cset))
	{
		client = accept_client(server);
		if (client)
		{
			FD_SET(client->fd, &server->set);
			client->logon_time = time(NULL);
			send_to(client, HELLO_STRING);
			printf("New connection. Hello message sent\n");
			client->id = server->last_id++;
			send_to(client, "You are the connection #%i width fd %i in this server.\n\rYour logon time is %lu\n\r", client->id, client->fd, client->logon_time);
			send_to(client, "To disconnect from server, press ctrl+c\n");
			server->clients_count++;
		}
	}
}
