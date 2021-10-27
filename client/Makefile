NAME	=	client
SRCS	=	main.c
OBJS	=	$(SRCS:.c=.o)
LDFLAGS	=	-lreadline

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

fclean:	clean
	$(RM) $(NAME)

clean:
	$(RM) $(OBJS)


.PHONY:	fclean clean all

