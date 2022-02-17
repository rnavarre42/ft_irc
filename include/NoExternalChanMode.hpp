#ifndef NOEXTERNALCHANMODE_HPP
# define NOEXTERNALCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class NoExternalChanMode : public AChanMode
{
public:
	NoExternalChanMode(Server &server);
	~NoExternalChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message);
	void onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
};

#endif
