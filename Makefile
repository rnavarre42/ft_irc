NAME		=	server
SRCSPATH	=	src/
OBJSPATH	=	obj/
OBJS		=	$(patsubst $(SRCSPATH)%, $(OBJSPATH)%, $(SRCS:.cpp=.o))
DEPS		=	$(OBJS:.o=.d)
CFLAGS		=	-Wall -Wextra -Werror -MD -I$(INCLUDEPATH) $(COMMONFLAGS) -std=c++98
COMMONFLAGS	=	
LDFLAGS		=	$(COMMONFLAGS)
INCLUDEPATH	=	./include/
FSANITIZE	=	-g3 -fsanitize=address
CC			=	clang++
RM			=	rm -Rf

-include	sources.mk

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

release:	COMMONFLAGS += -O3
release:	$(NAME)
	strip $(NAME)

tag:
	ctags	$(SRCS)

run:		$(NAME)
	./$(NAME)

cli:
	nc localhost 6667

hispano:
	make -C client run non NoSoyNadie 195.234.61.209 6667

dalnet:
	make -C client run non NoSoyNadie 143.244.34.1 6667

.SILENT:	clean fclean tag release print
.PHONY:		all clean fclean re debug tag release
