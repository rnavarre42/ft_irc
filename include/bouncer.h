#ifndef BOUNCER_H
# define BOUNCER_H

# include <sys/socket.h>
# include <netinet/in.h>

# define BUFFERSIZE			1024

# define SELECTTIMEOUT		1
# define NICKLEN			40
# define REALNAME			20
# define SERVER_PORT 		6667
# define MAXCLIENTS			3
# define MAXCOMMANDS		4

typedef struct	s_client
{
	unsigned long	id;
	int				fd;
	char			nick[NICKLEN];
	char			realname[REALNAME];
	uint64_t		logon_time;
	uint64_t		idle;
}				t_client;

typedef struct  s_message
{
	char	*name;
	char	**argv;
	char	*msg;
}				t_message;

struct s_server;

typedef struct	s_command
{
	char	*name;
	void	(*exec)(struct s_server *server, t_client *client, t_message *msg);
}				t_command;

typedef	struct	s_server
{
	uint64_t			last_id;
	int					last_fd;
	int					fd;
	int					opt;
	struct sockaddr_in	address;
	int					addrlen;
	fd_set				set;
	fd_set				cset;
	struct timeval		timeout;
	int					selecttimeout;
	int					clients_count;
	char				*ip;
	int					port;
	_Bool				stop;
	t_client			clients[MAXCLIENTS];
	t_command			commands[MAXCOMMANDS];
}				t_server;

void 	usr_quit(t_server *server, t_client *client, t_message *msg);
void 	usr_connect(t_server *server, t_client *client, t_message *msg);
void	usr_user(t_server *server, t_client *client, t_message *msg);
void	usr_nick(t_server *server, t_client *client, t_message *msg);

#endif
