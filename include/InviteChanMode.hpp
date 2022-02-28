#ifndef INVITECHANMODE_HPP
# define INVITECHANMODE_HPP
 
# include "AChanMode.hpp"

# include <string>

class Message;

class InviteChanMode : public AChanMode
{
public:
	InviteChanMode(Server& server);
	~InviteChanMode(void);

	void	onChanEvent(Access& access, int event, Message& message, int& numeric);
	bool	onChanModeEvent(int pos, int sign, Channel& channel, Message& message);
	void	onShowChanModeEvent(void);

	void		onDelete(void* pointer);
	std::string	toString(void* pointer);
};

#endif
