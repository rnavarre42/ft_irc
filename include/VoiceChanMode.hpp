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
	void onEnableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value);
	void onDisableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
