NAME		=	delegate
CXX			=	g++
CXXFLAGS	=	-Werror -Wextra -Wall $(COMMONFLAGS)
MDFLAGS		=	$(COMMONFLAGS)
COMMONFLAGS	=
RM			=	rm -rf
SRCS		=	main.cpp
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

re:			fclean all

run:
	./$(NAME)

.PHONY:		re all clean fclean run
