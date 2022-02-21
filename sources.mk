BASE_PATH		=	src/
BASE_SRCS		=	\
					ircserv.cpp					\
					utils.cpp					\
					ACommand.cpp				\
					Channel.cpp					\
					Console.cpp					\
					Invite.cpp					\
					ISender.cpp					\
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
					ModeCommand.cpp				\
					MotdCommand.cpp				\
					NamesCommand.cpp			\
					NickCommand.cpp				\
					PartCommand.cpp				\
					PassCommand.cpp				\
					PingCommand.cpp				\
					PongCommand.cpp				\
					PrivmsgCommand.cpp			\
					QuitCommand.cpp				\
					ShutdownCommand.cpp			\
					TopicCommand.cpp			\
					UserCommand.cpp				\

#					ListCommand.cpp				\
#					NoticeCommand.cpp			\
#					WhoCommand.cpp				\
#					WhoisCommand.cpp			\
#					WhowasCommand.cpp			\

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS)				\
						$(addprefix $(COMMAND_PATH), $(COMMAND_SRCS))	\
						$(addprefix $(MODES_PATH), $(MODES_SRCS))		\
					)													\
