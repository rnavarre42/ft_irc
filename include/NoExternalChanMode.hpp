#ifndef NOEXTERNALCHANMODE_HPP
# define NOEXTERNALCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class NoExternalChanMode : public AChanMode
{
public:
	NoExternalChanMode(Server& server);
	~NoExternalChanMode(void);

	void		onChanEvent(Access& access, int event, Message& message);
	bool		onChanModeEvent(int pos, int sign, Channel& channel, Message& message);
	void		onShowChanModeEvent(void);
	void		onDelete(void* pointer);
	std::string toString(void* pointer);
};

#endif
