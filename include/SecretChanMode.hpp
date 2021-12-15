#ifndef SECRETCHANMODE_HPP
# define SECRETCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class SecretChanMode : public AChanMode
{
public:
	SecretChanMode(Server &server);
	~SecretChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(Access &access, std::string &value);
	void onDisableChanModeEvent(Access &access, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
