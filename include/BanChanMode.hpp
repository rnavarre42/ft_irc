#ifndef BANCHANMODE_HPP
# define BANCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class BanChanMode : public AChanMode
{
public:
	BanChanMode(Server &server);
	~BanChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message);
	void onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
};

#endif
