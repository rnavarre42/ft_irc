#include "bouncer.h"

void	fill_commands(t_command *commands)
{
	commands[0] = (t_command){"QUIT", usr_quit};
	commands[1] = (t_command){"CONNECT", usr_connect};
	commands[2] = (t_command){"USER", usr_user};
	commands[3] = (t_command){"NICK", usr_nick};
}

void	usr_quit(t_server *server, t_client *client, t_message *msg)
{		
}

void	usr_connect(t_server *server, t_client *client, t_message *msg)
{
}

void	usr_user(t_server *server, t_client *client, t_message *msg)
{
}

void	usr_nick(t_server *server, t_client *client, t_message *msg)
{
}

