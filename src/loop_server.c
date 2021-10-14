#include <stdio.h>
#include <stdlib.h>
#include "bouncer.h"

void	loop_server(t_server *server)
{
	int rv;

	while (!server->stop)
	{
		rv = select_server(server);
		if (rv == -1)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}
		else if (rv == 0)
		{
			//check_idle(server);
			//check ping timeout
			//check new connection timeout
		}
		else
		{
			read_console(server);
			read_clients(server);
			check_client_connection(server);
		}
	}
}
