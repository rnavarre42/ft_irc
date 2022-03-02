#ifndef EXCEPTCHANMODE_HPP
# define EXCEPTCHANMODE_HPP
 
# include "AChanMode.hpp"

# include <string>

class Message;

class ExceptChanMode : public AChanMode
{
public:
	ExceptChanMode(Server& server);
	~ExceptChanMode(void);

	void	onChanEvent(Access& access, int event, Message& message, int& numeric);
	bool	onChanModeEvent(int pos, int sign, Channel& channel, Message& message);
	void	onShowChanModeEvent(Channel& channel, Message& message);

	void		onDelete(void* pointer);
	std::string toString(void* pointer);
};

#endif
