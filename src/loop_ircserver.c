void	loop_ircserver(ircserver)
{
	int 		rv;
	t_server	*ircserver;

	while (!ircserver->stop)
	{
		rv = select_ircserver(ircserver);
		if (rv == -1)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}
		else if (rv == 0)
		{
			//check_idle(ircserver);
			//check ping timeout
			//check new connection timeout
		}
		else
		{
			read_clients(ircserver);
			accept_pendings_clients(ircserver);
		}
	}
	free(ircserver);
}
