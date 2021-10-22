#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

#include <string>
#include <map>
#include <ctime>
#include <netinet/in.h>
#include <poll.h>

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

# define MAXUSERS		1
# define MAXLISTEN		5
class User;
class Channel;

class Server
{
	public:
		Server(std::string ip, int port);
		~Server(void);

		static Server	&getInstance(void);
		static Server	&getInstance(std::string ip, int port);
		void	start(void);
		void	sendTo(std::string msg);
		void	quit(std::string msg);

		class ServerFullException : std::exception
		{
			virtual const char	*what(void) const throw();
		};
	private:
		Server(void);

		std::string							ip;
		int									port;
		int									fd;
		std::map<std::string, User *>		users;
		std::map<std::string, Channel *>	channels;
		bool								stop;
		struct sockaddr_in					address;
		int									addrlen;
		int									opt;
		int									timeout;
		struct pollfd						pollfds[MAXUSERS + 1];
		static Server						*instance;


		int		findFreePollIndex(void);
		void	closeClients(std::string msg);
//		void	signalHandler(int sig);
		void	loop(void);
		void	initSocket(void);
		void	_bind(void);
		int		_poll(void);
		void	_listen(void);
		int		checkUserConnection(void);
		void	checkUserInput(void);
		User	&_accept();
};

#endif
