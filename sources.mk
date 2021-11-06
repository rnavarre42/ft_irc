BASE_PATH		=	src/
BASE_SRCS		=	\
					main.cpp					\
					utils.cpp					\
					ACommand.cpp				\
					Channel.cpp					\
					Console.cpp					\
					ISender.cpp					\
					Message.cpp					\
					Numeric.cpp					\
					Server.cpp					\
					User.cpp					\
#					Unknown.cpp					\

COMMAND_PATH	=	commands/
COMMAND_SRCS	=	\
					AwayCommand.cpp				\
					JoinCommand.cpp				\
					KickCommand.cpp				\
					KillCommand.cpp				\
					MotdCommand.cpp				\
					NickCommand.cpp				\
					PartCommand.cpp				\
					PassCommand.cpp				\
					PingCommand.cpp				\
					PongCommand.cpp				\
					PrivmsgCommand.cpp			\
					QuitCommand.cpp				\
					UserCommand.cpp				\

#					InviteCommand.cpp			\
#					ListCommand.cpp				\
#					ModeCommand.cpp				\
#					NamesCommand.cpp			\
#					NoticeCommand.cpp			\
#					TopicCommand.cpp			\
#					WhoCommand.cpp				\
#					WhoisCommand.cpp			\
#					WhowasCommand.cpp			\

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS)				\
						$(addprefix $(COMMAND_PATH), $(COMMAND_SRCS))	\
					)													\
