#ifndef CHANMODECONFIG_HPP
# define CHANMODECONFIG_HPP

# define COMMAND_JOIN		0x001
# define COMMAND_PART		0x002
# define COMMAND_PRIVMSG	0x004
# define COMMAND_NOTICE		0x008
# define COMMAND_INVITE		0x010
# define COMMAND_KICK		0x020
# define COMMAND_TOPIC		0x040
# define COMMAND_NICK		0x080
# define COMMAND_MODE		0x100

/*
mode #canal +ntslk 5 ii94ut

mode #canal +n

mode #canal +l 40
mode #canal -i

mode #canal +b nick!ident@host
mode #canal -b nick!ident@host

mode #canal +k pass
mode #canal -k pass

mode #canal +o nick
mode #canal +v nick
*/

struct ChanModeConfig
{
	ChanModeConfig()
	{}

	~ChanModeConfig()
	{}

	enum ModeType
	{
		noParam = 0x00,
		enableParam = 0x01,
		disableParam = 0x02
	};
	int			type;
	char		mode;
	int			events;
	bool		unique;

private:
	ChanModeConfig(ChanModeConfig const &src);
	ChanModeConfig &operator=(ChanModeConfig const &rhs);

};

#endif
