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

COMMAND_PATH	=	commands/
COMMAND_SRCS	=	\
					AwayCommand.cpp				\
					InviteCommand.cpp			\
					JoinCommand.cpp				\
					KickCommand.cpp				\
					KillCommand.cpp				\
					MotdCommand.cpp				\
					NamesCommand.cpp			\
					NickCommand.cpp				\
					PartCommand.cpp				\
					PassCommand.cpp				\
					PingCommand.cpp				\
					PongCommand.cpp				\
					PrivmsgCommand.cpp			\
					QuitCommand.cpp				\
					UserCommand.cpp				\

#					ListCommand.cpp				\
#					ModeCommand.cpp				\
#					NoticeCommand.cpp			\
#					TopicCommand.cpp			\
#					WhoCommand.cpp				\
#					WhoisCommand.cpp			\
#					WhowasCommand.cpp			\

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS)				\
						$(addprefix $(COMMAND_PATH), $(COMMAND_SRCS))	\
					)													\

CLIENT_PATH		=	src/
CLIENT_SRC		=	\
					irclient.cpp				\
					Client.cpp					\

CLIENT_SRCS		=	$(addprefix $(CLIENT_PATH), $(CLIENT_SRC)			\
					)													\
