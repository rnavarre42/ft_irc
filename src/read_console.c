#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "bouncer.h"

void	read_console(t_server *server)
{
	char	buffer[BUFFERSIZE];
	ssize_t	size;	

	if (FD_ISSET(0, &server->cset))
	{
		size = read(0, buffer, BUFFERSIZE);
		if (size == -1)
		{
			perror("console");
			exit(EXIT_FAILURE);
		}
		buffer[size] = '\0';
		send_all(server, "server> %s\r", buffer);
	}
}
