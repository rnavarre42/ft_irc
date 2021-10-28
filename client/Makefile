NAME	=	client
SRCS	=	main.c
OBJS	=	$(SRCS:.c=.o)
LDFLAGS	=	-lreadline -lpthread

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

fclean:		clean
	$(RM) $(NAME)

clean:
	$(RM) $(OBJS)

re:			fclean $(NAME)

run:		$(NAME)
	./$(NAME) $(filter-out $@,$(MAKECMDGOALS))

.SILENT: run
.PHONY:	fclean clean all re run

