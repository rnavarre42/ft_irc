BASE_PATH		=	src/
BASE_SRCS		=	\
					main.cpp					\
					Server.cpp					\
					User.cpp					\
					Channel.cpp					\
					Console.cpp					\
					ACommand.cpp				\
					Message.cpp					\
					ISender.cpp					\

COMMAND_PATH	=	commands/
COMMAND_SRCS	=	\
					UserCommand.cpp				\

SRCS			=	$(addprefix $(BASE_PATH), $(BASE_SRCS)				\
						$(addprefix $(COMMAND_PATH), $(COMMAND_SRCS))	\
					)													\
