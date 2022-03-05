#ifndef ASENDER_HPP
# define ASENDER_HPP

# include <string>
# include <sys/types.h>

# define LEVEL_UNREGISTERED	0x01
# define LEVEL_NEGOTIATING	0x02
# define LEVEL_REGISTERED	0x04
# define LEVEL_IRCOPERATOR	0x08
# define LEVEL_ALL			LEVEL_UNREGISTERED | LEVEL_NEGOTIATING | LEVEL_REGISTERED | LEVEL_IRCOPERATOR

# define TYPE_UNKNOWN		0x00
# define TYPE_LOCALSERVER	0x01
# define TYPE_LOCALUSER		0x02
# define TYPE_REMOTESERVER	0x03
# define TYPE_REMOTEUSER	0x04

class Server;
class Message;

class ASender
{
public:
	virtual ~ASender(void);

	void						setPass(const std::string& value);
	const std::string&			getPass(void) const;

	void						setName(const std::string& value);
	const std::string&			getName(void) const;

	void						setHost(const std::string& value);
	const std::string&		 	getHost(void) const;

	void						setReal(const std::string& value);
	const std::string&		 	getReal(void) const;

	void						setLevel(int value);
	int							getLevel(void);

	int							getFd(void) const;

	const std::string&			getMask(void) const;
	int							getType(void) const;

	void						setNextTimeout(time_t value);
	time_t						getNextTimeout(void) const;

	void						setIdleTime(time_t value);
	time_t						getIdleTime(void) const;

	void						setIdleInnerTime(time_t value);
	time_t						getIdleInnerTime(void) const;

	void						setPingChallenge(const std::string& value) ;
	const std::string&			getPingChallenge(void) const;
	
	virtual ssize_t				send(void) = 0;
	virtual ssize_t				send(const std::string& data) = 0;
	virtual ssize_t				send(const Message& message) = 0;

	size_t						recv(void);
	size_t						checkInput(Message& message);
	bool						checkOutput(void);

protected:
	ASender(void);
	ASender(Server& server, int fd, time_t signTime, int level, int type, const std::string& pass, const std::string& name, const std::string& host, const std::string& real);

	std::string					_getLine(size_t pos);
	void						_doUpdateMask(void);
	virtual void				_updateMask(void) = 0;
	
	Server&		_server;
	int			_fd;

	time_t		_signTime;
	time_t		_nextTimeout;
	time_t		_idleInnerTime;
	time_t		_idleTime;

	int			_level;
	int			_type;
	
	std::string	_pass;
	std::string _name;
	std::string	_host;
	std::string	_real;


	std::string	_mask;
	std::string	_pingChallenge;
	
	std::string	_inputBuffer;
	std::string	_outputBuffer;

};

#endif
