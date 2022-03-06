#ifndef NUMERICS_HPP
# define NUMERICS_HPP

enum
{
	RPL_WELCOME				=   1,	// "Welcome to the Internet Relay Network <nick>!<user>@<host>"
	RPL_YOURHOST			=   2,	// "Your host is <servername>, running version <ver"
	RPL_CREATED				=   3,	// "This server was created <date>"
	RPL_MYINFO				=   4,	// "<servername> <version> <available user modes> <available channel modes>"
	RPL_ISUPPORT			=   5,	// "$ :are supported by this server"

	RPL_USERHOST			= 302,	// ":*1<reply> *( ' ' <reply> )"
	RPL_ISON				= 303,	// ":*1<nick> *( ' ' <nick> )"

	RPL_AWAY				= 301,	// "<nick> :<away message>"
	RPL_UNAWAY				= 305,	// ":You are no longer marked as being away"
	RPL_NOAWAY				= 306,	// ":You have been marked as being away"

	RPL_WHOISUSER			= 311,	// "<nick> <user> <host> * :<real name>"
	RPL_WHOISSERVER			= 312,	// "<nick> <server> :<server info>"
	RPL_WHOISOPERATOR		= 313,	// "<nick> :is an IRC operator"

	RPL_WHOISIDLE			= 317,	// "<nick> <integer> :seconds idle"
	RPL_ENDOFWHOIS			= 318,	// "<nick> :End of WHOIS list"
	RPL_WHOISCHANNELS		= 319,	// "<nick> :*( ( '@' / '+' ) <chhannel> ' ' )"

	RPL_WHOWASUSER			= 314,	// "<nick> <user> <host> * :<real name>"
	RPL_ENDOFWHOWAS			= 369,	// "<nick> :End of WHOWAS"

	RPL_LISTSTART			= 321,	// obsolete
	RPL_LIST				= 322,	// "<channel> <# visible> :<topic>"
	RPL_LISTEND				= 323,	// ":End of LIST"

	RPL_UNIQOPIS			= 325,	// "<channel> <nickname>"
	RPL_CHANNELMODEIS		= 324,	// "<channel> <mode> <mode params>"
	RPL_NOTOPIC				= 331,	// "<channel> :No topic is set"
	RPL_TOPIC				= 332,	// "<channel> :<topic>"
	RPL_TOPICTIME			= 333,	// "<channel> :<own> <time>"

	RPL_INVITING			= 341,	// "<channel> <nick>"
	RPL_SUMMONING			= 342,	// "<user> :Summoning user to IRC"
	RPL_INVITELIST			= 346,	// "<channel> <invitemask>"
	RPL_ENDOFINVITELIST		= 347,	// "<channel> :End of channel invite list"
	RPL_EXCEPTLIST			= 348,	// "<channel> :End of channel exception list"

	RPL_VERSION				= 351,	// "<version>,<debuglevel> <server> :<comments>"
	RPL_WHOREPLY			= 352,	// "<channel> <user> <server> <nick> ( 'H' / 'G' > ['*'] [ ( '@' / '+' ) ] :<hopcount> <real name>"
	RPL_ENDOFWHO			= 315,	// "<name> :End of WHO list"

	RPL_NAMREPLY			= 353,	// "( '=' / '+' / '@' ) <channel> :[ '@' / '+' ] <nick> *( ' ' [ '@' / '+' ] <nick> )"
	RPL_ENDOFNAMES			= 366,	// "<channel> :End of NAMES list"

	RPL_LINKS				= 364,	// "<mask> <server> :<hopcount> <server info>"
	RPL_ENDOFLINKS			= 365,	// "<mask> :End of LINKS list"

	RPL_BANLIST				= 367,	// "<channel> <banmask>"
	RPL_ENDOFBANLIST		= 368,	// "<channel> :End of channel ban list"

	RPL_INFO				= 371,	// ":<string>"
	RPL_ENDOFINFO			= 374,	// ":End of INFO list"

	RPL_MOTDSTART			= 375,	// ":- <server> Message of the day - "
	RPL_MOTD				= 372,	// ":- <text>
	RPL_ENDOFMOTD			= 376,	// ":End of MOTD command"

	RPL_WHOISHOST			= 378,	// "<nick> :is connection from <user mask> <real host>"
	RPL_WHOISMODES			= 379,	// "<nick> :is using modes +<modes>

	RPL_YOUREOPER			= 381,	// ":You are now an IRC operator"
	RPL_REHASHING			= 382,	// "<config file> :Rehashing"
	//RPL_YOURESERVICE		= 383,	// "

	ERR_TRYAGAIN			= 263,	// "<command> :Please wait a while and try again"

	ERR_NOSUCHNICK			= 401,	// "<nickname> :No such nick/channel"
	ERR_NOSUCHSERVER		= 402,	// "<server name> :No such server"
	ERR_NOSUCHCHANNEL		= 403,	// "<channel> :No such channel"
	ERR_CANNOTSENDTOCHAN	= 404,	// "<channel name> :Cannot send to channel"
	ERR_TOOMANYCHANNELS		= 405,	// "<channel name> :You have joined too many channels"
	ERR_WASNOSUCHNICK		= 406,	// "<nickname> :There was no such nickname"
	ERR_TOOMANYTARGETS		= 407,	// "<target> :<error code> recipients. <abort message>"
	ERR_NOSUCHSERVICE		= 408,	// "<service name> :No such service"
	ERR_NOORIGIN			= 409,	// ":No origin specified"

	ERR_NORECIPIENT			= 411,	// ":No recipient given (<command>)"
	ERR_NOTEXTTOSEND		= 412,	// ":No text to send"
	ERR_NOTOPLEVEL			= 413,	// "<mask> :No toplevel domain specified"
	ERR_WILDTOPLEVEL		= 414,	// "<mask> :Wildcard in toplevel domain"
	ERR_BADMASK				= 415,	// "<mask> :Bad server/host mask"

									// custom numeric, only for ft_irc
	ERR_NOTIMPLEMENTED		= 420,  // "<command> :Command not implemented yet"

	ERR_UNKNOWNCOMMAND		= 421,	// "<command> :Unknown command"
	ERR_NOMOTD				= 422,	// ":MOTD File is missing"
	ERR_NOADMININFO			= 423,	// "<server> :No administrative info available"
	ERR_FILEERROR			= 424,	// "File error doing <file op> on <file>"

	ERR_NONICKNAMEGIVEN		= 431,	// ":No nickname given"
	ERR_ERRONEUSNICKNAME	= 432,	// "<nick> :Erroneus nickname"
	ERR_NICKNAMEINUSE		= 433,	// "<nick> :Is already in use"

	ERR_NICKCOLLISION		= 436,	// "<nick> :Nickname collision KILL from <user>@<host>"
	ERR_UNAVAILRESOURCE		= 437,	// "<nick/channel> :Nick/channel is temporary unavailable"

	ERR_USERNOTINCHANNEL	= 441,	// "<nick> <channel> :They aren't on that channel"
	ERR_NOTONCHANNEL		= 442,	// "<channel> :You're not on that channel"
	ERR_USERONCHANNEL		= 443,	// "<user> <channel> :is already on channel"
	ERR_NOLOGIN				= 444,	// "<user> :User not logged in"
	ERR_SUMMONDISABLED		= 445,	// ":SUMMON has been disabled"
	ERR_USERSDISABLED		= 446,	// ":USERS has been disabled"
	ERR_CANTCHANGENICK		= 447,  // ":Cannot change nickname while on <channel> (you're banned)"

	ERR_NOTREGISTERED		= 451,	// ":You have not registered"

	ERR_NEEDMOREPARAMS		= 461,	// "<command> :Not enough parameters"
	ERR_ALREADYREGISTERED	= 462,	// ":Unauthorized command (already registered)"
	ERR_NOPERMFORHOST		= 463,	// ":Your host isn't among the privileged"
	ERR_PASSWDMISMATCH		= 464,	// ":Password incorrect"
	ERR_YOUREBANNEDCREEP	= 465,	// ":You are banned from this server"
	ERR_YOUWILLBEBANNED		= 466,	// ""
	ERR_KEYSET				= 467,	// "<channel> :Channel key already set"

	ERR_CHANNELISFULL		= 471,	// "<channel> :Cannot join channel (+l)"
	ERR_UNKNOWNMODE			= 472,	// "<char> :is unknown mode char to me for <channel>"
	ERR_INVITEONLYCHAN		= 473,	// "<channel> :Cannot join channel (+i)"
	ERR_BANNEDFROMCHAN		= 474,	// "<channel> :Cannot join channel (+b)"
	ERR_BADCHANNELKEY		= 475,	// "<channel> :Cannot join channel (+k)"
	ERR_BADCHANMASK			= 476,	// "<channel> :Bad channel Mask"
	ERR_NOCHANMODES			= 477,	// "<channel> :Channel doesn't support modes"
	ERR_BANLISTFULL			= 478,	// "<channel> <char> :Channel list is full"

	ERR_NOPRIVILEGES		= 481,	// ":Permision Denied- You're not an IRC operator"
	ERR_CHANOPRIVSNEEDED	= 482,	// "<channel> :You're not channel operator"
	ERR_CANTKILLSERVER		= 483,	// ":You can't kill a server!"
	ERR_RESTRICTED			= 484,	// ":Your connection is restricted!"
	ERR_UNIQOPPRIVSNEEDED	= 485,	// ":You're not the original channel operator"

	ERR_NOOPERHOST			= 491,	// "No O-lines for your host"

	ERR_UMODEUNKNOWNFLAG	= 501,	// "Unknown MODE flag"
	ERR_USERSDONTMATCH		= 502	// ":Cannot change mode for other users"

};

#endif
