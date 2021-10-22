NAME		=	server
SRCSPATH	=	src/
OBJSPATH	=	obj/
SRCS		=	main.cpp					\
				Server.cpp					\
				User.cpp					\
				Channel.cpp					\

SRCS		:=	$(addprefix $(SRCSPATH), $(SRCS))
OBJS		=	$(patsubst $(SRCSPATH)%, $(OBJSPATH)%, $(SRCS:.cpp=.o))
DEPS		=	$(OBJS:.o=.d)
CFLAGS		=	-Wall -Wextra -Werror -MD -I$(INCLUDEPATH) -g3 $(COMMONFLAGS) -std=c++98
COMMONFLAGS	=	
LDFLAGS		=	$(COMMONFLAGS)
INCLUDEPATH	=	./include/
FSANITIZE	=	-fsanitize=address
CC			=	g++
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

debug:		LDFLAGS += $(FSANITIZE)
debug:		$(NAME)

tag:
	ctags	$(SRCS)

run:		$(NAME)
			./$(NAME)

.SILENT:	clean fclean tag
.PHONY:		all clean fclean re debug tag
