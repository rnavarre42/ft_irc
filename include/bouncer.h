#ifndef BOUNCER_H
# define BOUNCER_H

# include <sys/socket.h>
# include <netinet/in.h>

# define PORT		6667

typedef struct	s_bouncer
{
	int					server_fd;
	int					client_fd;
	struct sockaddr_in	server_addr;
	int					opt;
	int					addrlen;
	char				buffer[1024];
}				t_bouncer;

#endif
