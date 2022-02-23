#ifndef VOICECHANMODE_HPP
# define VOICECHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class VoiceChanMode : public AChanMode
{
public:
	VoiceChanMode(Server& server);
	~VoiceChanMode(void);

	void onChanEvent(Access& access, int event, Message& message);
	bool onChanModeEvent(int pos, int sign, Channel& channel, Message& message);
	void onShowChanModeEvent(void);
	void onDelete(void* pointer);
	std::string toString(void* pointer);
};

#endif
