#ifndef VOICECHANMODE_HPP
# define VOICECHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class VoiceChanMode : public AChanMode
{
public:
	VoiceChanMode(Server &server);
	~VoiceChanMode(void);

	void onChanEvent(Access &access, Message &message);
	bool onChanModeEvent(int order, int sign, Access &access, User &user, Channel &channel, Message &message);
//	void onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
};

#endif
