#ifndef CHANMODECONFIG_HPP
# define CHANMODECONFIG_HPP

# define CHANMODE_JOIN		0x001
# define CHANMODE_PART		0x002
# define CHANMODE_PRIVMSG	0x004
# define CHANMODE_NOTICE	0x008
# define CHANMODE_INVITE	0x010
# define CHANMODE_KICK		0x020
# define CHANMODE_TOPIC		0x040
# define CHANMODE_NICK		0x080
# define CHANMODE_MODE		0x100

enum ModeType
{
	bothParam,
	enableParam,
	disableParam
};

struct ChanModeConfig
{
	ModeType	type;
	char		mode;
	int			events;
};

#endif
