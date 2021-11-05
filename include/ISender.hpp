#ifndef ISENDER_HPP
# define ISENDER_HPP

//# include "Message.hpp"
# include <string>
# include <sys/types.h>

# define TYPE_SERVER		0x00
# define TYPE_USER			0x01

class Message;

class ISender
{
public:
	virtual ~ISender(void);

	virtual std::string const	&getName(void) const = 0;
	virtual int const			&getFd(void) const = 0;
	virtual std::string const	&getPass(void) const = 0;
	virtual void				setPass(std::string value) = 0;
	virtual ssize_t				send(std::string msg = "") = 0;
	virtual ssize_t				send(Message &message) = 0;
	virtual bool const			&isRegistered(void) const = 0;
	virtual bool				isServer(void) = 0;
	virtual bool				isUser(void) = 0;
	virtual bool				isOper(void) = 0;
	virtual int					getType(void) = 0;
	virtual std::string			getMask(void) = 0;
	virtual void				setIdleTime(time_t value) = 0;
};

#endif
