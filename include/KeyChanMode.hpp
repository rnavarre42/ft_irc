#ifndef KEYCHANMODE_HPP
# define KEYCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class KeyChanMode : public AChanMode
{
public:
	KeyChanMode(Server &server);
	~KeyChanMode(void);

	void onChanEvent(Access &access, Message &message);
	bool onChanModeEvent(int pos, int sign, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
};

#endif
