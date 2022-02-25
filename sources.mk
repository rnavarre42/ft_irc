BASE_PATH		=	src/
BASE_SRCS		=	\
					ircserv.cpp					\
					utils.cpp					\
					ACommand.cpp				\
					Channel.cpp					\
					Console.cpp					\
					Invite.cpp					\
					Message.cpp					\
					Numeric.cpp					\
					Server.cpp					\
					User.cpp					\
#					Unknown.cpp					\

MODES_PATH		=	modes/
MODES_SRCS		=	\
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
					PartCommand.cpp				\
					PassCommand.cpp				\
					PingCommand.cpp				\
					PongCommand.cpp				\
					PrivmsgCommand.cpp			\
					QuitCommand.cpp				\
					ShutdownCommand.cpp			\
					TopicCommand.cpp			\
					UserCommand.cpp				\

#					WhoCommand.cpp				\
#					WhoisCommand.cpp			\
#					WhowasCommand.cpp			\

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS)				\
						$(addprefix $(COMMAND_PATH), $(COMMAND_SRCS))	\
						$(addprefix $(MODES_PATH), $(MODES_SRCS))		\
					)													\


CLIENT_PATH		=	src/
CLIENT_SRCS		=	\
					irclient.cpp				\
					Client.cpp					\
					Readline.cpp				\


CSRCS			=	$(addprefix $(CLIENT_PATH), $(CLIENT_SRCS)			\
					)
