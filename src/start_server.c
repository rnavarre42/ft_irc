#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "bouncer.h"

void	signal_handler(int signal)
{
	t_server	*server;

	if (signal == SIGINT)
	{
		server = singleton_server();
		printf("Disconnecting clients...\n");
		disconnect_all(server, "Shutdown server. Please, reconnect to another server.");
		exit(EXIT_SUCCESS);
	}
}

t_server	*start_server(char *ip, int port, int timeout)
{
	t_server *server;

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
	server = init_server(ip, port, timeout);
	fill_commands(server->commands);
	if (!server)
	{
		perror("init_server");
		exit(EXIT_FAILURE);
	}
	bind_server(server);
	loop_server(server);
	free(server);
	return (server);
}
