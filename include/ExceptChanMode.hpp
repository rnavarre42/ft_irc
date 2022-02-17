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
	void onEnableChanModeEvent(Access &access, User &user, Channel &channel, Message &message);
	void onDisableChanModeEvent(Access &access, User &user, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
};

#endif
