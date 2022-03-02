#ifndef USER_HPP
# define USER_HPP

# include "ISender.hpp"
# include "Server.hpp"
# include "utils.hpp"
# include "Channel.hpp"

# include <string>
# include <map>

class AUserMode;
class Message;
class ISender;
class Unknown;

class User : public ISender
{
public:
	User(int fd, Server& server);
	User(const Unknown& src);
	~User(void);

	void				setHost(const std::string& value);
	const std::string&	getHost(void) const;

	void				setIdent(const std::string& value);
	const std::string&	getIdent(void) const;

	void				setReal(const std::string& value);
	const std::string&	getReal(void) const;
	const std::string&	getMask(void) const;

	void				setPass(const std::string& value);
	const std::string&	getPass(void) const;

	void				setSignTime(time_t volue);
	const time_t&		getSignTime(void) const;
		
	void				setName(const std::string& value);
	const std::string&	getName(void) const;
	const std::string&	getUpperName(void) const;

	void				insert(Channel* channel);
	void				erase(Channel* channel);

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

	Server::channelMap_iterator	find(const std::string& value)
	{
		return this->_channelMap.find(strToUpper(value));
	}

	Channel*&	operator[](const std::string& channelName)
	{
		return this->_channelMap.at(strToUpper(channelName));
	}

	bool	operator==(const std::string& rhs)
	{
		return (strToUpper(this->_name) == strToUpper(rhs));
	}

	bool	operator!=(const std::string& rhs)
	{
		return (!operator==(rhs));
	}

	bool	operator==(const User& rhs)
	{
		return (*this == rhs._name);
	}

	bool	operator!=(const User& rhs)
	{
		return (!operator==(rhs));
	}

	std::string&	getInputBuffer(void);
	std::string&	getOutputBuffer(void);

	bool	isUser(void);
	bool	isServer(void);
	bool	isOper(void);
	void	setOper(bool value);
	bool	isOnChannel(const Channel& channel);
	bool	isOnChannel(const std::string& channel);

	int	getType(void);

	void				clearPingChallenge();
	void				setPingChallenge(const std::string& value);
	const std::string&	getPingChallenge(void) const;

	void				setAwayMsg(const std::string& value);
	const std::string&	getAwayMsg(void) const;
	const time_t&		getAwayTime(void) const;

	const Server&	getServer(void) const;

	void 			setNextTimeout(time_t value);
	const time_t&	getNextTimeout(void) const;

	void			setIdleTime(time_t value);
	const time_t&	getIdleTime(void) const;

	void	setStatus(int value);
	int		getStatus(void);

	void		setPollIndex(int value);
	const int&	getPollIndex(void) const;

	void		setFd(int value);
	const int&	getFd(void) const;

	bool	isPingSended(void);
	bool	isAway(void);

	void	setMode(AUserMode* userMode);
	void	unsetMode(AUserMode* userMode);
	bool	isSetMode(AUserMode* userMode);

	void	sendPing(void);
	ssize_t	send(void);
	ssize_t	send(const std::string& text);
	ssize_t	send(const Message& message);
	void	sendToBuffer(std::string msg);
	void	sendToBuffer(const Message& message);

	size_t	recv(int fd);

	Message*	buildMessage(std::string& buff);


	std::size_t	checkInput(int fd, Message& message);
	bool		checkOutput(int fd);

	Server::userVector_type*	getUniqueVector(void);
	Channel*					findFullestChannel(void);

private:
	User(void);
	User(const User& src);
	User&	operator=(const User& rhs);

	Server&					_server;

	Server::channelMap_type	_channelMap;

	std::string	_name;
	std::string	_host;
	std::string	_ident;
	std::string	_real;
	std::string	_pass;
	std::string	_inputBuffer;
	std::string	_outputBuffer;
	std::string _awayMsg;
	std::string	_pingChallenge;
	std::string	_mask;

	int			_status;
	std::time_t	_signTime;
	std::time_t	_awayTime;
	std::time_t	_nextTimeout;
	std::time_t	_idleTime;
	int			_pollIndex;
	int			_fd;
	int			_type;
	bool		_oper;
	uint64_t	_modes;

	std::string	_getLine(size_t pos);
	void		_updateMask(void);
};
#endif
