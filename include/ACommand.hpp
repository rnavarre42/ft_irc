#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Server.hpp"

# include <string>

//class Server;
class Message;
class User;

template <class EvenT, class ValueT> 
class EventHandler;

class ACommand
{
public:
	ACommand(Server& server, int levelAccess, int minParam);
	virtual ~ACommand(void);

	virtual void	loadEvents(Server::eventHandler_type& eventHandler) = 0;
	virtual void	unloadEvents(Server::eventHandler_type& eventHandler) = 0;

	void	recv(Message &msg);
	void	send(Message &msg);

private:
	virtual bool	_recvUser(Message& message) = 0;
	virtual bool	_recvServer(Message& message) = 0;
	virtual bool	_sendUser(Message& message) = 0;
	virtual bool	_sendServer(Message& message) = 0;

	ACommand(void);
	ACommand(const ACommand& src);
	ACommand&	operator=(const ACommand& rhs);

protected:
	Server&		server;
	int			levelAccess;
	size_t		minParam;
	Server*		senderServer;
	User*		senderUser;
	Server*		receiverServer;
	User*		receiverUser;
};

#endif
