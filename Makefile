NAME		=	irc_bouncer
SRCSPATH	=	src/
OBJSPATH	=	obj/
SRCS		=	main.c				\

SRCS		:=	$(addprefix $(SRCSPATH), $(SRCS))
OBJS		=	$(patsubst $(SRCSPATH)%, $(OBJSPATH)%, $(SRCS:.c=.o))
DEPS		=	$(OBJS:.o=.d)
CFLAGS		=	-Wall -Wextra -Werror -MD -I$(INCLUDEPATH)
LDFLAGS		=	
INCLUDEPATH	=	./include/
FSANITIZE	=	-fsanitize=address
CC			=	gcc
RM			=	rm -Rf

-include	$(DEPS)

all:	$(NAME)

$(OBJSPATH)%.o:	$(SRCSPATH)%.c
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

.SILENT:	clean fclean tag
.PHONY:		all clean fclean re debug tag
