#ifndef TOPICCHANMODE_HPP
# define TOPICCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class TopicChanMode : public AChanMode
{
public:
	TopicChanMode(Server &server);
	~TopicChanMode(void);

	void 		onChanEvent(Access &access, Message &message);
	bool		onChanModeEvent(int pos, int sign, Channel &channel, Message &message);
	void		onShowChanModeEvent(void);
	void		onDelete(void *pointer);
	std::string toString(void *pointer);
};

#endif
