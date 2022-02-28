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

struct ChanModeConfig
{
	ChanModeConfig(void)
	{}

	~ChanModeConfig(void)
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
	ChanModeConfig(const ChanModeConfig& src);
	ChanModeConfig&	operator=(const ChanModeConfig& rhs);
};

#endif
