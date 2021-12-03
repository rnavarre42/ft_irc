NAME		=	client
SRCS		=	main.cpp \
				Client.cpp
OBJS		=	$(SRCS:.cpp=.o)

CXX			=	clang++
CXXFALGS	=	-Wall -Wextra -Werror $(COMMONFLAGS)
COMMONFLAGS	=
LDFLAGS		=	$(COMMONFLAGS)
FSANITIZE	=	-g3 -fsanitize=address

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

debug:		COMMONFLAGS = $(FSANITIZE)
debug:		all

run:		all
	./$(NAME) localhost 6667
