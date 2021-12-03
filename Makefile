NAME		=	ircserv
SRCSPATH	=	src/
OBJSPATH	=	obj/
OBJS		=	$(patsubst $(SRCSPATH)%, $(OBJSPATH)%, $(SRCS:.cpp=.o))
DEPS		=	$(OBJS:.o=.d)
CXXFLAGS	=	-Wall -Wextra -Werror -MD -I$(INCLUDEPATH) $(COMMONFLAGS) -std=c++98
COMMONFLAGS	=	
LDFLAGS		=	$(LDLIBS) $(COMMONFLAGS)
LDLIBS		=	
INCLUDEPATH	=	./include/
FSANITIZE	=	-g3 -fsanitize=address
CXX			=	clang++
RM			=	rm -Rf

-include	sources.mk

all:	debug
#$(NAME)

-include	$(DEPS)

$(OBJSPATH)%.o:	$(SRCSPATH)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(NAME):	$(OBJS)
	$(CXX) -o $(NAME) $(OBJS) $(LDFLAGS)

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

release:	COMMONFLAGS = -O3
release:	$(NAME)
	strip $(NAME)

tag:
	ctags	$(SRCS)

run:		all
	./$(NAME)

cli:
	make -C client run non NoSoyNadie 127.0.0.1 6667
	reset

hispano:
	make -C client run non NoSoyNadie 195.234.61.209 6667

dalnet:
	make -C client run non NoSoyNadie 143.244.34.1 6667

.SILENT:	clean fclean tag release print
.PHONY:		all clean fclean re debug tag release
