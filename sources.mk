BASE_PATH		=	src/
BASE_SRCS		=	\
					main.cpp					\
					utils.cpp					\
					Server.cpp					\
					User.cpp					\
					Channel.cpp					\
					Console.cpp					\
					ACommand.cpp				\
					Message.cpp					\
					ISender.cpp					\
					Numeric.cpp					\

COMMAND_PATH	=	commands/
COMMAND_SRCS	=	\
					AwayCommand.cpp				\
					InviteCommand.cpp			\
					JoinCommand.cpp				\
					KickCommand.cpp				\
					KillCommand.cpp				\
					ListCommand.cpp				\
					ModeCommand.cpp				\
					NamesCommand.cpp			\
					NickCommand.cpp				\
					NoticeCommand.cpp			\
					PartCommand.cpp				\
					PassCommand.cpp				\
					PingCommand.cpp				\
					PongCommand.cpp				\
					PrivmsgCommand.cpp			\
					QuitCommand.cpp				\
					TopicCommand.cpp			\
					UserCommand.cpp				\
					WhoCommand.cpp				\
					WhoisCommand.cpp			\
					WhowasCommand.cpp			\

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS)				\
						$(addprefix $(COMMAND_PATH), $(COMMAND_SRCS))	\
					)													\
