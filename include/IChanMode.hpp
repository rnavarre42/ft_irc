#ifndef ICHANMODE_HPP
# define ICHANMODE_HPP
 
#include <string>

class Message;
struct ChanModeConfig;

enum ModeAccess
{
	def,
	deny,
	allow
};

class IChanMode
{
public:
	virtual void loadChanMode(ChanModeConfig &chanModeConfig) = 0;
//	virtual void unloadChanMode(void);
	virtual void onChanEvent(ModeAccess &modeAccess, Message &message) = 0;
	virtual void onEnableChanModeEvent(ModeAccess &modeAccess, std::string &value) = 0;
	virtual void onDisableChanModeEvent(ModeAccess &modeAccess, std::string &value) = 0;
	virtual void onShowChanModeEvent(void) = 0;
};

#endif
