#ifndef USER_HPP
# define USER_HPP

# include "ISender.hpp"
# include "Server.hpp"
# include "utils.hpp"
# include "Channel.hpp"

# include <string>
# include <map>

class Message;
class ISender;

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

	void				setSignTime(time_t volue);
	time_t const		&getSignTime(void) const;
		
	void				setName(std::string value);
	std::string const	&getName(void) const;
	std::string const	&getUpperName(void) const;

	void				insert(Channel *channel);
	void				erase(Channel *channel);

	Server::channelMap_iterator	begin(void)
	{
		return this->_channelMap.begin();
	}

	Server::channelMap_iterator	end(void)
	{
		return this->_channelMap.end();
	}

	Server::channelMap_type::size_type	size(void)
	{
		return this->_channelMap.size();
	}

	Server::channelMap_iterator	find(std::string &value)
	{
		return this->_channelMap.find(strToUpper(value));
	}

	Channel	*&operator[](std::string channelName)
	{
		return this->_channelMap.at(strToUpper(channelName));
	}

	std::string &getInputBuffer(void);
	std::string &getOutputBuffer(void);

	bool		isUser(void);
	bool		isServer(void);
	bool		isOper(void);
	bool		isOnChannel(Channel &channel);
	bool		isOnChannel(std::string &channel);

	int			getType(void);

	void				clearPingChallenge();
	void				setPingChallenge(std::string value);
	std::string const	&getPingChallenge(void) const;

	void				setAwayMsg(std::string value);
	std::string const	&getAwayMsg(void) const;

	//Server::channelMap_type		&getChannelMap(void);
	Server const				&getServer(void) const;

	void 			setAwayTime(time_t value);
	time_t const	&getAwayTime(void) const;

	void 			setNextTimeout(time_t value);
	time_t const	&getNextTimeout(void) const;

	void			setIdleTime(time_t value);
	time_t const	&getIdleTime(void) const;

	void		setStatus(int value);
	int			getStatus(void);

	void		setPollIndex(int value);
	int const	&getPollIndex(void) const;

	void		setFd(int value);
	int	const	&getFd(void) const;

	bool	isPingSended(void);
	bool	isAway(void);

	void		sendPing(void);
	ssize_t		send(std::string msg = "");
	ssize_t		send(Message &message);
	void		sendToBuffer(std::string msg);
	void		sendToBuffer(Message &message);

	size_t	recv(int fd);

	Message	*buildMessage(std::string &buff);
	size_t	checkInput(int fd, Message &message);
	bool	checkOutput(int fd);

	Server::userVector_type	*getUniqueVector(void);
	Channel					*findFullestChannel(void);

	bool	operator==(std::string &rhs)
	{
		return (strToUpper(this->_name) == strToUpper(rhs));
	}

	bool	operator!=(std::string &rhs)
	{
		return !operator==(rhs);
	}

private:
	User(void);
	User(User const &src);
	User &operator=(User const &rhs);

	std::string					_name;
	std::string					_host;
	std::string					_ident;
	std::string					_real;
	std::string					_pass;
	std::string					_inputBuffer;
	std::string					_outputBuffer;
	std::string 				_awayMsg;
	std::string					_pingChallenge;
	Server						&_server;
	Server::channelMap_type		_channelMap;
	int							_status;
	time_t						_signTime;
	time_t						_awayTime;
	time_t						_nextTimeout;
	time_t						_idleTime;
	int							_pollIndex;
	int							_fd;
	int							_type;

	std::string					_getLine(size_t pos);
};

/*
inline bool operator==(User const &lhs, User const &rhs)
{
	return (&lhs == &rhs);
}

inline bool operator==(std::string const &lhs, User const &rhs)
{
	return (strToUpper(rhs.getName()) == strToUpper(lhs));
}

inline bool operator==(User const &lhs, std::string const &rhs)
{
	return (strToUpper(lhs.getName()) == strToUpper(rhs));
}
*/
#endif
