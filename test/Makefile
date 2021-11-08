NAME		=	delegate
CXX			=	g++
CXXFLAGS	=	-Werror -Wextra -Wall $(COMMONFLAGS)
MDFLAGS		=	$(COMMONFLAGS)
COMMONFLAGS	=
RM			=	rm -rf
SRCS		=	main.cpp Server.cpp JoinCommand.cpp PartCommand.cpp QuitCommand.cpp
OBJS		=	$(SRCS:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(MDFLAGS) $(OBJS) -o $@

debug:	COMMONFLAGS += -fsanitize=address
debug:	CXXFLAGS += -g3
debug:	$(NAME)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

run:
	./$(NAME)

.PHONY:		all clean fclean run
