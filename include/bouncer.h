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

#define  HELLO_STRING		"\033[31;1;4mHello from server\033[0m\n"

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
	char		*name;
	void		*source;
	void		*target;
	char		**argv;
	char		*msg;
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

t_client	*accept_client(t_server *server);
void		check_client_connection(t_server *server);
void		bind_server(t_server *server);
t_client	*get_client_slot(t_server *server);
int			get_highest_fd(t_client *clients);
t_server	*init_server(char *ip, int port, int timeout);
void		loop_server(t_server *server);
void		read_clients(t_server *server);
void		read_console(t_server *server);
int			select_server(t_server *server);
ssize_t		send_to(t_client *client, char *data, ...);
ssize_t		send_all(t_server *server, char *data, ...);
ssize_t		send_except(t_server *server, t_client *sender, char *data, ...);
t_server	*start_server(char *ip, int port, int timeout);
_Bool		telnet_ctrlc(char *data);
t_server	*singleton_server(void);
void		disconnect_client(t_server *server, t_client *client);
void		disconnect_all(t_server *server, char *msg);
t_message	*build_message(t_server *server, t_client *client, char *data);

void 	usr_quit(t_server *server, t_client *client, t_message *msg);
void 	usr_connect(t_server *server, t_client *client, t_message *msg);
void	usr_user(t_server *server, t_client *client, t_message *msg);
void	usr_nick(t_server *server, t_client *client, t_message *msg);

#endif
