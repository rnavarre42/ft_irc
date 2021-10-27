NAME		=	server
SRCSPATH	=	src/
OBJSPATH	=	obj/
SRCS		=	main.cpp					\
				Server.cpp					\
				User.cpp					\
				Channel.cpp					\
				Console.cpp					\
				CommandBase.cpp				\
				Message.cpp					\
				UserCommand.cpp				\
				ISender.cpp					\



SRCS		:=	$(addprefix $(SRCSPATH), $(SRCS))
OBJS		=	$(patsubst $(SRCSPATH)%, $(OBJSPATH)%, $(SRCS:.cpp=.o))
DEPS		=	$(OBJS:.o=.d)
CFLAGS		=	-Wall -Wextra -Werror -MD -I$(INCLUDEPATH) $(COMMONFLAGS) -std=c++98
COMMONFLAGS	=	-O3
LDFLAGS		=	$(COMMONFLAGS)
INCLUDEPATH	=	./include/
FSANITIZE	=	-g3 -fsanitize=address
CC			=	clang++
RM			=	rm -Rf


all:	$(NAME)

-include	$(DEPS)

$(OBJSPATH)%.o:	$(SRCSPATH)%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME):	$(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(DEPS)

fclean:		clean
	$(RM) $(NAME) tags

print:
	echo $(SRCS)
	echo $(OBJS)

re:			fclean all

debug:		COMMONFLAGS = $(FSANITIZE)
debug:		$(NAME) tag

release:	$(NAME)
	strip $(NAME)

tag:
	ctags	$(SRCS)

run:		$(NAME)
	./$(NAME)

cli:
	nc localhost 6667

.SILENT:	clean fclean tag release
.PHONY:		all clean fclean re debug tag release
