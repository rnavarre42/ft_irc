BASE_PATH		=	src/
BASE_SRCS		=	\
					ircserv.cpp					\
					utils.cpp					\
					ACommand.cpp				\
					AUserMode.cpp				\
					Channel.cpp					\
					Console.cpp					\
					Invite.cpp					\
					Message.cpp					\
					Numeric.cpp					\
					Server.cpp					\
					User.cpp					\
					Unknown.cpp					\

CHANMODES_PATH	=	chanmodes/
CHANMODES_SRCS	=	\
					BanChanMode.cpp				\
					ExceptChanMode.cpp			\
					InviteChanMode.cpp			\
					KeyChanMode.cpp				\
					LimitChanMode.cpp			\
					ModerateChanMode.cpp		\
					NoExternalChanMode.cpp		\
					OperChanMode.cpp			\
					SecretChanMode.cpp			\
					TopicChanMode.cpp			\
					VoiceChanMode.cpp			\

USERMODES_PATH	=	usermodes/
USERMODES_SRCS	=	\
					OperUserMode.cpp			\
					InvisibleUserMode.cpp		\


COMMAND_PATH	=	commands/
COMMAND_SRCS	=	\
					AwayCommand.cpp				\
					InviteCommand.cpp			\
					JoinCommand.cpp				\
					KickCommand.cpp				\
					KillCommand.cpp				\
					ListCommand.cpp				\
					ModeCommand.cpp				\
					MotdCommand.cpp				\
					NamesCommand.cpp			\
					NickCommand.cpp				\
					NoticeCommand.cpp			\
					OperCommand.cpp				\
					PartCommand.cpp				\
					PassCommand.cpp				\
					PingCommand.cpp				\
					PongCommand.cpp				\
					PrivmsgCommand.cpp			\
					QuitCommand.cpp				\
					ShutdownCommand.cpp			\
					TopicCommand.cpp			\
					UserCommand.cpp				\
					WhoisCommand.cpp			\

#					WhoCommand.cpp				\
#					WhowasCommand.cpp			\

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS)					\
						$(addprefix $(COMMAND_PATH), $(COMMAND_SRCS))		\
						$(addprefix $(CHANMODES_PATH), $(CHANMODES_SRCS))	\
						$(addprefix $(USERMODES_PATH), $(USERMODES_SRCS))	\
					)														\


CLIENT_PATH		=	src/
CLIENT_SRCS		=	\
					irclient.cpp				\
					Client.cpp					\
					Readline.cpp				\


CSRCS			=	$(addprefix $(CLIENT_PATH), $(CLIENT_SRCS)			\
					)
