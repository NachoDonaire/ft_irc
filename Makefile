SRCS = main.cpp	\
       Server.cpp \
       Client.cpp \
       Command.cpp
SRDIR = src/
NAME = ircserv
DSRC = $(addprefix $(SRDIR), $(SRCS))
ALLC = $(DSRC)
OBJS = $(DSRC:.cpp=.o)
CXX = c++ -Wall -Werror -std=c++98 -Iinc
RM = rm -rf

all: $(NAME)

$(NAME) : $(OBJS)
	$(CXX) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
