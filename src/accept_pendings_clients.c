#include "irc_bouncer.h"

void	accept_pendings_clients(t_server *ircserver)
{
	t_client	*client;

	if (FD_ISSET(ircserver->fd, &ircserver->cset))
	{
		client = ircserver->clients + ircserver->clients_count;
		if ((client->fd = accept(ircserver->fd, (struct sockaddr *)&ircserver->address, (socklen_t *)&ircserver->addrlen)) < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		if (ircserver->clients_count < MAXCLIENTS)
		{
			FD_SET(client->fd, &ircserver->set);
			client->logon_time = time(NULL);
			send_to(client, HELLO_STRING);
			printf("New connection. Hello message sent\n");
			client->id = ircserver->last_id++;
			send_to(client, "You are the connection #%i width fd %i in this server.\n\rYour logon time is %lu\n\r", client->id, client->fd, client->logon_time);
			send_to(client, "To disconnect from server, press ctrl+c\n");
			send_to(client, "server> ");
			ircserver->clients_count++;
		}
		else
		{
			send_to(client, "The server is full. Please, try again more later.\n\r");
			close(client->fd);
		}
	}
}
