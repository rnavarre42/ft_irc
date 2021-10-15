#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <map>
# include "Server.hpp"

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

		std::map<std::string, Channel *>	&getChannels(void) const;
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

#endif
