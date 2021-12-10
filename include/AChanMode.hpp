#ifndef ACHANMODE_HPP
# define ACHANMODE_HPP
 
#include <string>

class Message;

class AChannelMode
{
public:
	virtual void loadChanMode(void);
	virtual void unloadChanMode(void);
	virtual void onChanEvent(Access &access, Message &message) = 0;
	virtual void onEnableChanModeEvent(Access &access, std::string) = 0;
	virtual void onDisableChanModeEvent(Access &access, std::string) = 0;
}

#endif
