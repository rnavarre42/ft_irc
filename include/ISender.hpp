#ifndef ISENDER_HPP
# define ISENDER_HPP

# include <string>
# include <sys/types.h>

# define TYPE_SERVER		0x0
# define TYPE_USER		0x1

class ISender
{
public:
	virtual ~ISender(void);

	virtual std::string const	&getName(void) const = 0;
	virtual int const			&getFd(void) const = 0;
	virtual std::string const	&getPass(void) const = 0;
	virtual void				setPass(std::string value) = 0;
	virtual ssize_t				send(std::string msg) = 0;
	virtual bool const			&isRegistered(void) const = 0;
	virtual bool				isServer(void) = 0;
	virtual bool				isUser(void) = 0;
	virtual bool				isOper(void) = 0;
	virtual int					getType(void) = 0;
	virtual std::string			getMask(void) = 0;
};

#endif
