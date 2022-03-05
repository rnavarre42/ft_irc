#ifndef USER_HPP
# define USER_HPP

# include "ASender.hpp"
# include "Server.hpp"
# include "utils.hpp"
# include "Channel.hpp"

# include <string>
# include <map>

class AUserMode;
class Message;
class ASender;
class Unknown;

class User : public ASender
{
public:
	User(Server& server, int fd);
	User(Server& server, time_t signTime, const std::string& pass, const std::string& name, const std::string& ident, const std::string& host, const std::string& real);
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

	bool	operator==(std::string& rhs)
	{
		return (strToUpper(this->_name) == strToUpper(rhs));
	}

	bool	operator!=(std::string& rhs)
	{
		return !operator==(rhs);
	}

	std::string&	getInputBuffer(void);
	std::string&	getOutputBuffer(void);

	bool	isOper(void);
	void	enableOper(void);
	bool	isOnChannel(const Channel& channel);
	bool	isOnChannel(const std::string& channel);

	void				clearPingChallenge();

	void				setAwayMsg(const std::string& value);
	const std::string&	getAwayMsg(void) const;
	const time_t&		getAwayTime(void) const;

	void		setPollIndex(int value);
	const int&	getPollIndex(void) const;

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

	size_t	checkInput(int fd, Message& message);
	bool	checkOutput(int fd);

	Server::userVector_type*	getUniqueVector(void);
	Channel*					findFullestChannel(void);

private:
	User(void);
	User(const User& src);
	User&	operator=(const User& rhs);

	Server::channelMap_type	_channelMap;

	std::string	_ident;
	std::string _awayMsg;

	std::time_t	_awayTime;
	int			_pollIndex;
	bool		_oper;
	uint64_t	_modes;

	std::string	_getLine(size_t pos);
	void		_updateMask(void);
};
#endif
