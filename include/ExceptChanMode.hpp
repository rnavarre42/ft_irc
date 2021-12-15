#ifndef EXCEPTCHANMODE_HPP
# define EXCEPTCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class ExceptChanMode : public AChanMode
{
public:
	ExceptChanMode(Server &server);
	~ExceptChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(Access &access, std::string &value);
	void onDisableChanModeEvent(Access &access, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
