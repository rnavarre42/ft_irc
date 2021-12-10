#ifndef BANCHANMODE_HPP
# define BANCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class BanChanMode
{
public:
	void loadChanMode(ChanModeConfig &chanModeConfig);
	void onChanEvent(ModeAccess &modeAccess, Message &message);
	void onEnableChanModeEvent(ModeAccess &modeAccess, std::string &value);
	void onDisableChanModeEvent(ModeAccess &modeAccess, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
