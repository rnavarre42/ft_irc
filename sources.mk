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
					KillCommand.cpp				\
					MotdCommand.cpp				\
					PassCommand.cpp				\
					PingCommand.cpp				\
					PongCommand.cpp				\
					PrivmsgCommand.cpp			\
					NickCommand.cpp				\
					UserCommand.cpp				\
					QuitCommand.cpp				\

#					AwayCommand.cpp				\
#					InviteCommand.cpp			\
#					JoinCommand.cpp				\
#					KickCommand.cpp				\

#					ListCommand.cpp				\
#					ModeCommand.cpp				\
#					NamesCommand.cpp			\
#					NoticeCommand.cpp			\
#					PartCommand.cpp				\
#					TopicCommand.cpp			\
#					WhoCommand.cpp				\
#					WhoisCommand.cpp			\
#					WhowasCommand.cpp			\

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS)				\
						$(addprefix $(COMMAND_PATH), $(COMMAND_SRCS))	\
					)													\
