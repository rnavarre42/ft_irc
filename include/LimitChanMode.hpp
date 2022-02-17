#ifndef LIMITCHANMODE_HPP
# define LIMITCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class LimitChanMode : public AChanMode
{
public:
	LimitChanMode(Server &server);
	~LimitChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(Access &access, User &user, Channel &channel, Message &message);
	void onDisableChanModeEvent(Access &access, User &user, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
};

#endif
