#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Server.hpp"

# include <string>
# include <vector>

class ACommand;
class ISender;
class User;
class Server;

class Message
{
public:
	typedef std::vector<std::string>		paramVector_type;
	typedef std::vector<ISender *>			receiverVector_type;

	Message(Server &server);
	~Message(void);

	std::string			&operator[](size_t index);

	void				setCmd(const std::string& value);
	const std::string&	getCmd(void) const;

	void				set(ISender& sender, std::string data);
	void				setReceiver(const Channel* channel);
	void				setReceiver(const Server::userVector_type& userVector);
	void				setReceiver(const ISender* value);
	void				hideReceiver(void);
	ISender*			getReceiver(void);

	void				clear(void);
	void				clearReceiver(void);

	void				setSender(ISender* value);
	ISender*			getSender(void);
	Server*				getServer(void);

	void				setChannel(Channel* value);
	Channel*			getChannel(void);

	void				reply(void);
	void				reply(const std::string& data);
	void				replyNumeric(int numeric);
	void				send(void);
	void				send(const std::string& data);
	void				sendNumeric(int numeric);
	void				process(void);

	size_t				size(void);
	bool				empty(void);
	void				eraseAt(size_t index);

	void				limitMaxParam(size_t limit);

	Message::paramVector_type*	getParamVector(void)
	{
		return &_paramVector;
	}

	void				swapField(size_t first, size_t second);
	void				insertField(std::string field);
	std::string			toString(void) const;
//	static Message&		builder(ISender &sender, std::string data);
//	static Message&		builder(ISender &sender);

private:
	Message(ISender &sender, std::string data);
	Message(ISender &sender);

	Server&							_server;
	ISender*						_sender;
	Channel*						_channel;
	Message::receiverVector_type	_receiverVector;
	std::string 					_prefix;
	std::string 					_cmd;
	ACommand*						_command;
	Message::paramVector_type		_paramVector;

	bool							_hideReceiver;
};

#endif
