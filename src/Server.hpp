#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

#include <string>
#include <map>
#include <ctime>

# define LOG_CONNECT		0x0001

# define DEBUG		LOG_CONNECT

# define MAXNICK		9
# define MAXCHANNEL		50
# define MAXSERVER		63
# define MAXTOPIC		307
# define MAXLINE		512
# define MAXHOST		63
# define MAXAWAY		200
# define MAXQUIT		255
# define MAXKICK		255
# define MAXREAL		100

# define MAXUSERS		5

class Server
{
	public:
		Server(std::string ip, int port);
		~Server(void);

		void	start(void);
		void	send(std::string msg);
		void	quit(std::string msg);
	private:
		Server(void);

		char								*ip;
		int									port;
		int									fd;
		std::map<std::string, User *>		clients;
		std::map<std::string, Channel *>	channels;
		bool								stop;
		struct sockaddr_in					*address;
		int									*addrlen;
		int									opt;
		struct pollfd						pollfds[MAXUSERS + 1];

	
		int		findFreePollIndex(void);
		void	closeClients(std::string msg);
		void	signalHandler(int sig);
		void	loop(void);
		void	initSocket(void);
		void	bind(void);
		void	listen(void);
		User	&accept();
}

#endif
