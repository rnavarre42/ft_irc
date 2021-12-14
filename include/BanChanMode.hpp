#ifndef BANCHANMODE_HPP
# define BANCHANMODE_HPP
 
#include "IChanMode.hpp"

#include <string>

class Message;

class BanChanMode : public IChanMode
{
public:
	void loadChanMode(ChanModeConfig &chanModeConfig);
	void onChanEvent(ModeAccess &modeAccess, Message &message);
	void onEnableChanModeEvent(ModeAccess &modeAccess, std::string &value);
	void onDisableChanModeEvent(ModeAccess &modeAccess, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
