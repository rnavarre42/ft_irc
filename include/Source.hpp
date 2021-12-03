#ifndef SOURCE_CPP
# define SOURCE_CPP

class Server;
class Channel;
class Message;

struct Source1
{
	Server	*server;
	Channel	*channel;
	Message *message;
};

#endif
