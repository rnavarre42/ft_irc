#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define __STDC_WANT_LIB_EXT2__ 1 
#define _GNU_SOURCE
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "bouncer.h"

#define  HELLO_STRING "\033[31;1;4mHello from server\033[0m\n"

_Bool	telnet_ctrlc(char *buffer)
{
	char exit[] = {255, 244, 255, 253, 6};

	return (!strncmp(buffer, exit, 5));
}

ssize_t	send_to(t_client *client, char *data, ...)
{
	va_list ap;
	char	*strp;
	int		len;
	ssize_t	ret;

	if (!client->fd)
		return (-1);
	va_start(ap, data);
	len = vasprintf(&strp, data, ap);
	ret = send(client->fd, strp, len, 0);
	free(strp);
	return (ret);
}

int	get_max_fd(t_client *clients)
{
	int	max_fd;

	max_fd = clients[0].fd;
	for (int i = 1; i < MAXCLIENTS; i++)
	{
		if (max_fd < clients[i].fd)
			max_fd = clients[i].fd;
	}
	return (max_fd);
}

t_server	*init_ircserver(char *ip, int port, int timeout)
{
	t_server	*ircserver;

	ircserver = malloc(sizeof(t_server));
	if (!ircserver)
		return (NULL);
	bzero(ircserver, sizeof(t_server));
	ircserver->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (!ircserver->fd)
	{
		perror("Create socket failed");
		exit(EXIT_FAILURE);
	}
	ircserver->opt = 1;
	ircserver->ip = ip;
	ircserver->port = port;
	ircserver->selecttimeout = timeout;
	ircserver->addrlen = sizeof(ircserver->address);
	if (setsockopt(ircserver->fd, SOL_SOCKET, SO_REUSEADDR
			, &ircserver->opt, sizeof(ircserver->opt)))
	//		| SO_REUSEPORT <-- mac compatibility
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	return (ircserver);
}

void		bind_ircserver(t_server *ircserver)
{
	ircserver->address.sin_family = AF_INET;
	ircserver->address.sin_addr.s_addr = INADDR_ANY;
	ircserver->address.sin_port = htons(ircserver->port);
	if (bind(ircserver->fd, (struct sockaddr *)&ircserver->address,
			sizeof(ircserver->address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(ircserver->fd, 3) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	FD_ZERO(&ircserver->set);
	FD_SET(ircserver->fd, &ircserver->set);
}

int	select_ircserver(t_server *ircserver)
{
	memcpy(&ircserver->cset, &ircserver->set, FD_SETSIZE / 8);
	ircserver->last_fd = get_max_fd(ircserver->clients);
	if (ircserver->last_fd < ircserver->fd)
		ircserver->last_fd = ircserver->fd;
	ircserver->timeout.tv_sec = ircserver->selecttimeout;
	ircserver->timeout.tv_usec = 0;
	return (select(ircserver->last_fd + 2, &ircserver->cset,
		NULL, NULL, &ircserver->timeout));
}

t_server	*start_ircserver(char *ip, int port, int timeout)
{
	t_server *ircserver;

	signal(SIGPIPE, SIG_IGN);
	ircserver = init_ircserver(ip, port, timeout);
	if (!ircserver)
	{
		perror("init_ircserver");
		exit(EXIT_FAILURE);
	}
	bind_ircserver(ircserver);
	loop_ircserver(ircserver);
	return (ircserver);
}

int main(void)
{
	printf("Waiting connect clients...\n");
	start_ircserver("0.0.0.0", 6667, SELECTTIMEOUT);
}
