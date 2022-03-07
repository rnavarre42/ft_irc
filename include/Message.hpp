#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Server.hpp"

# include <string>
# include <vector>
# include <exception>

class ACommand;
class ASender;
class User;
class Server;

class Message
{
public:
	typedef std::vector<ASender*>				receiverVector_type;
	typedef receiverVector_type::iterator		receiverVector_iterator;
	typedef std::vector<std::string>			paramVector_type;
	typedef paramVector_type::iterator			paramVector_iterator;

	Message(Server& server);
	~Message(void);

	std::string&	operator[](size_t index);

	void				setCmd(const std::string& value);
	const std::string&	getCmd(void) const;

	void		set(ASender* sender, std::string data);
	void		setReceiver(Channel& channel);
	void		setReceiver(Server::userVector_type& userVector);
	void		setReceiver(ASender& value);
	void		hideReceiver(void);
	void		hideSemicolon(void);

	ASender&	getReceiver(void);

	void		clear(void);
	void		clearReceivers(void);

	void		setSender(ASender& value);

	ASender&	getSender(void);
	Server&		getServer(void);

	void		setChannel(Channel& value);
	Channel&	getChannel(void);

	void		reply(void);
	void		reply(const std::string& data);
	void		replyNumeric(int numeric); //its for reply a client
	void		send(void);
	void		send(const std::string& data);
	void		sendNumeric(int numeric); //for server only???? 
	void		internal(void); //for process internally command to send a new one.

	std::size_t	size(void);
	bool		empty(void);
	void		eraseAt(size_t index);
	void		limitMaxParam(size_t limit);

	std::vector<std::string>*	getParamVector(void)
	{
		return &_paramVector;
	}

	void		swapField(size_t first, size_t second);
	void		insertField(std::string field);
	std::string	toString(void) const;

//	static Message		&builder(ASender &sender, std::string data);
//	static Message		&builder(ASender &sender);

	class ReceiverIsEmpty : public std::exception
	{
		const char* what() const throw()
		{
			return "The message cannot be delivered. The receiver field is empty";
		}
	};

private:
	Message(ASender& sender, std::string data);
	Message(ASender& sender);

	Server&						_server;
	ASender*					_sender;
	Channel*					_channel;
	receiverVector_type			_receiverVector;
	paramVector_type			_paramVector;
	std::string 				_prefix;
	std::string 				_cmd;
	ACommand*					_command;
	bool						_hideReceiver;
	bool						_hideSemicolon;
};

#endif
