#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#include "bouncer.h"

#define PORT 6667

_Bool	exit_client(char *buffer)
{
	char exit[] = {255, 244, 255, 253, 6};

	return (!strncmp(buffer, exit, 5));
}

int main(void)
{
	int	server_fd;
	int new_socket;
	int valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "\033[31;1;4mHello from server\033[0m\n";
	int i;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (!server_fd)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET,
			SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	while (1)
	{
		send(new_socket, "server> ", 8, 0);
		valread = read(new_socket, buffer, 1024);
		buffer[valread] = 0;
		if (exit_client(buffer))
		{
			printf("Client close connection\n");
			exit(EXIT_SUCCESS);
		}
		printf("lenght: %i, \"%s\"", valread, buffer);
		i = 0;
		while (i < valread)
			printf("%hhu, ", buffer[i++]);
		printf("\n");
	}
	return (EXIT_SUCCESS);
}
