NAME		=	irc_bouncer
SRCSPATH	=	src/
OBJSPATH	=	obj/
SRCS		=	accept_client.c				\
				check_client_connection.c	\
				bind_server.c				\
				get_client_slot.c			\
				disconnect_client.c			\
				disconnect_all.c			\
				fill_commands.c				\
				get_highest_fd.c			\
				init_server.c				\
				loop_server.c				\
				main.c						\
				read_clients.c				\
				read_console.c				\
				select_server.c				\
				send_to.c					\
				send_all.c					\
				send_except.c				\
				start_server.c				\
				singleton_server.c			\
				build_message.c				\
				print_message.c				\
				poll_server.c


SRCS		:=	$(addprefix $(SRCSPATH), $(SRCS))
OBJS		=	$(patsubst $(SRCSPATH)%, $(OBJSPATH)%, $(SRCS:.c=.o))
DEPS		=	$(OBJS:.o=.d)
CFLAGS		=	-Wall -Wextra -Werror -MD -I$(INCLUDEPATH) -g3 $(COMMONFLAGS)
COMMONFLAGS	=	
LDFLAGS		=	$(COMMONFLAGS)
INCLUDEPATH	=	./include/
FSANITIZE	=	-fsanitize=address
CC			=	gcc
RM			=	rm -Rf


all:	$(NAME)

-include	$(DEPS)

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

run:		$(NAME)
			./$(NAME)

.SILENT:	clean fclean tag
.PHONY:		all clean fclean re debug tag
