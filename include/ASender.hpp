#ifndef ASENDER_HPP
# define ASENDER_HPP

# include <string>
# include <sys/types.h>

# define TYPE_UNKNOWN		0x00
# define TYPE_SERVER		0x01
# define TYPE_USER			0x02


class Server;
class Message;

class ASender
{
public:
	virtual ~ASender(void)
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
	virtual bool				isOper(void) = 0;
	virtual int					getType(void) = 0;
	//La máscara no la tenemos almacenada, devolver una referencia no tiene sentido, a no ser que hagamos
	//lo que propuso Miki, actualizar la máscara de cada usuario / servidor que ya tenemos en memoria.
	virtual const std::string&	getMask(void) const = 0;
	virtual size_t				checkInput(int fd, Message& message) = 0;
	virtual bool				checkOutput(int fd) = 0;
	virtual void				setNextTimeout(time_t value) = 0;
	virtual const time_t&		getNextTimeout(void) const = 0;
	virtual void				setIdleTime(time_t value) = 0;
	virtual const time_t&		getIdleTime(void) const = 0;
	virtual void				setPingChallenge(const std::string& value) = 0;
	virtual const std::string&	getPingChallenge(void) const = 0;
};

#endif
