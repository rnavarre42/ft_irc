#ifndef KEYCHANMODE_HPP
# define KEYCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class KeyChanMode : public AChanMode
{
public:
	KeyChanMode(Server &server);
	~KeyChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(Access &access, std::string &value);
	void onDisableChanModeEvent(Access &access, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
