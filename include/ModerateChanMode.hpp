#ifndef MODERATECHANMODE_HPP
# define MODERATECHANMODE_HPP
 
# include "AChanMode.hpp"

# include <string>

class Message;

class ModerateChanMode : public AChanMode
{
public:
	ModerateChanMode(Server& server);
	~ModerateChanMode(void);

	void	onChanEvent(Access& access, int event, Message& message, int& numeric);
	bool	onChanModeEvent(int pos, int sign, Channel& channel, Message& message);
	void	onShowChanModeEvent(Channel& channel, Message& message);

	void		onDelete(void* pinter);
	std::string toString(void* pointer);
};

#endif
