#ifndef EXCEPTCHANMODE_HPP
# define EXCEPTCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class ExceptChanMode : public AChanMode
{
public:
	ExceptChanMode(Server &server);
	~ExceptChanMode(void);

	void onChanEvent(Access &access, Message &message);
	bool onChanModeEvent(int pos, int sign, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
	void onDelete(void *pointer);
	std::string getValue(void *pointer);
};

#endif
