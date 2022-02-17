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
	void onEnableChanModeEvent(Access &access, User &user, Channel &channel, Message &message);
	void onDisableChanModeEvent(Access &access, User &user, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
};

#endif
