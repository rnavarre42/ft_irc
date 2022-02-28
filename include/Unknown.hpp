#ifndef UNKNOWN_HPP
# define UNKNOWN_HPP

# include "ISender.hpp"
# include "User.hpp"

# include <string>

class Server;

class Unknown : public ISender
{
public:
	Unknown(int fd, Server& server);
	~Unknown(void);

	const std::string&	getName(void) const;
	const int&			getFd(void) const;
	const std::string&	getPass(void) const;
	void				setPass(const std::string& value);
	ssize_t				send(void);
	ssize_t				send(const std::string& data);
	ssize_t				send(const Message& message);
	void				setStatus(int value);
	int					getStatus(void);
	bool				isServer(void);
	bool				isUser(void);
	bool				isOper(void);
	int					getType(void);
	const std::string&	getMask(void) const;

	friend 				User::User(const Unknown& src);

private:
	Unknown&	operator=(const Unknown& rhs);

	Server&				_server;
	std::string			_name;
	std::string			_pass;
	std::string			_ident;
	std::string			_mask;
	time_t				_signTime;
	time_t				_nextTimeout;
	int					_status;
	int					_fd;
	int					_type;

};

#endif
