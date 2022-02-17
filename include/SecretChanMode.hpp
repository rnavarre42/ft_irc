#ifndef SECRETCHANMODE_HPP
# define SECRETCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class SecretChanMode : public AChanMode
{
public:
	SecretChanMode(Server &server);
	~SecretChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(Access &access, User &user, Channel &channel, Message &message);
	void onDisableChanModeEvent(Access &access, User &user, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
};

#endif
