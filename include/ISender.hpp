#ifndef ISENDER_HPP
# define ISENDER_HPP

# include <string>
# include <sys/types.h>

# define TYPE_UNKNOWN		0x00
# define TYPE_SERVER		0x01
# define TYPE_USER			0x02


class Server;
class Message;

class ISender
{
public:
	virtual ~ISender(void)
	{}

	virtual const std::string&	getName(void) const = 0;
	virtual const int&			getFd(void) const = 0;
	virtual const std::string&	getPass(void) const = 0;
	virtual void				setPass(const std::string& value) = 0;
	virtual ssize_t				send(void) = 0;
	virtual ssize_t				send(const std::string& data) = 0;
	virtual ssize_t				send(const Message& message) = 0;
	virtual void				setStatus(int value) = 0;
	virtual int					getStatus(void) = 0;
	virtual bool				isServer(void) = 0;
	virtual bool				isUser(void) = 0;
	virtual int					getType(void) = 0;
	virtual const std::string&	getMask(void) const = 0;
	virtual void				setIdleTime(time_t value) = 0;
};

#endif
