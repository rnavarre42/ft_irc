#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <map>
# include "Channel.hpp"
# include "Server.hpp"
# include "ISender.hpp"
# include "Message.hpp"

class Message;
class ISender;
class Server;

class User : public ISender
{
public:
	User(int fd, Server &server);
	~User(void);

	void				setHost(std::string value);
	std::string const	&getHost(void) const;

	void				setIdent(std::string value);
	std::string const	&getIdent(void) const;

	void				setReal(std::string value);
	std::string const	&getReal(void) const;
	std::string			getMask(void);

	void				setPass(std::string value);
	std::string const	&getPass(void) const;

	void			setSignTime(time_t volue);
	time_t const	&getSignTime(void) const;
		
	void				setName(std::string value);
	std::string const	&getName(void) const;

	std::string &getInputBuffer(void);
	std::string &getOutputBuffer(void);

	bool		isUser(void);
	bool		isServer(void);
	bool		isOper(void);
	int			getType(void);

	void				clearPingChallenge();
	void				setPingChallenge(std::string value);
	std::string const	&getPingChallenge(void) const;

	void				setAwayMsg(std::string value);
	std::string const	&getAwayMsg(void) const;

	std::map<std::string, Channel *> &getChannelMap(void);
	Server const	&getServer(void) const;

	void 			setAwayTime(time_t value);
	time_t const	&getAwayTime(void) const;

	void 			setNextTimeout(time_t value);
	time_t const	&getNextTimeout(void) const;

	void			setIdleTime(time_t value);
	time_t const	&getIdleTime(void) const;

	void		setRegistered(bool value);
	bool const	&isRegistered(void) const;

	void		setPollIndex(int value);
	int const	&getPollIndex(void) const;

	void		setFd(int value);
	int	const	&getFd(void) const;

	bool	isPingSended(void);
	bool	isAway(void);

	void		sendPing(void);
	ssize_t		send(std::string msg);

	size_t	recv(int fd);

	Message	*buildMessage(std::string &buff);
	size_t	checkInput(int fd);
	bool	checkOutput(int fd);

private:
	User(void);

	std::string							name;
	std::string							host;
	std::string							ident;
	std::string							real;
	std::string							pass;
	std::string							inputBuffer;
	std::string							outputBuffer;
	std::string 						awayMsg;
	std::string							pingChallenge;
	Server								&server;
	std::map<std::string, Channel *>	channelMap;
	bool								registered;
	time_t								signTime;
	time_t								awayTime;
	time_t								nextTimeout;
	time_t								idleTime;
	int									pollIndex;
	int									fd;
	int									type;
};

#endif
