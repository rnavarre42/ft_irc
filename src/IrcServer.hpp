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

class Channel
{
	public:
		Channel(std::string name);
		~Channel(void);

		std::string						&getName(void) const;
		std::map<std::string, User *>	&getUsers(void) const;

		void							setCreator(std::string value);
		std::string						&getCreator(void) const;
	
		void							setTopic(std::string value);
		std::string						&getTopic(void) const;
		
		void							setTopicOwn(std::string value);
		std::string						&getTopicOwn(void) const;

		void							setTopicTime(time_t value);
		time_t							&getTopicTime(void) const;


	private:
		std::string						name;
		std::string						topic;
		std::string						creator;
		std::string						topicOwn;
		time_t							topicTime;
		std::map<std::string, User *>	users;

		Channel(void);
}

class User
{
	public:
		User(int fd, Server &server);
		~User(void);

		void		setHost(std::string value);
		std::string &getHost(void) const;

		void		setIdent(std::string value);
		std::string &getIdent(void) const;

		void		setReal(std::string value);
		std::string &getReal(void) const;

		void		setSignTime(time_t volue);
		time_t 		&getSignTime(void) const;
		
		void		setNick(std::string value);
		std::string	&getNick(void) const;

		void		setAwayMsg(std::string value);
		std::string &getAwayMsg(void) const;

		std:::map<std::string, Channel *>	&getChannels(void) const;
		Server								&getServer(void) const;

		void 	setAwayTime(time_t value);
		time_t	&getAwayTime(void) const;

		void 	setNextPing(time_t value);
		time_t	&getNextPing(void) const;

		void	setIdleTime(time_t value);
		time_t	&getIdleTime(void) const;

		void	setRegistered(bool value);
		bool	isRegistered(void);

		bool	isAway(void);
//		bool	isOper(void);


		int		send(std::string msg);
		void	disconnec(std::string msg);

	private:
		User(void);

		std::string							nick;
		std::string							host;
		std::string							ident;
		std::string							real;
		std::string 						awayMsg;
		Server								&server;
		std::map<std::string, Channel *>	channels;
		time_t								signTime;
		bool								registered;
		time_t								awayTime;
		time_t								nextPing;
		time_t								idleTime;
		int									fd;
}

class Server
{
	public:
		Server(std::string ip, int port);
		~Server(void);

		void	start(void);

	private:
		Server(void);

		char							*ip;
		int								port;
		int								listen_fd;
		std::map<std::string, User>		clients;
		std::map<std::string, Channel>	channels;
		bool							stop;


		void	closeClients(std::string msg);
		void	signalHandler(int sig);
		void	loop(void);
		User	&accept(); //aqui se tiene que añadir el fd a poll y a la lista de conexiones. (map)
}

#endif
