#ifndef ICHANNELEVENT_CPP
# define ICHANNELEVENT_CPP

class Message;

class IChannelEvent
{
public:
//	AChannelEvent(void) {}
	virtual void	preChannelEvent(Message &message, bool &cancel) = 0;
//	virtual void	postChannelEvent(Message &message, bool &cancel) = 0;
};

#endif
