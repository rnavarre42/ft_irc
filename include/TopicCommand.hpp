#ifndef TOPICCOMMAND_HPP
# define TOPICCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class TopicCommand : public ACommand
{
public:
	TopicCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
