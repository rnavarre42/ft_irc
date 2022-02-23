#ifndef LIMITCHANMODE_HPP
# define LIMITCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class LimitChanMode : public AChanMode
{
public:
	LimitChanMode(Server& server);
	~LimitChanMode(void);

	void onChanEvent(Access& access, int event, Message& message);
	bool onChanModeEvent(int pos, int sign, Channel& channel, Message& message);
	void onShowChanModeEvent(void);
	void onDelete(void* pointer);
	std::string toString(void* pointer);
};

#endif
