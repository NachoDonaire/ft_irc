SRCS = main.cpp	\
       Server.cpp \
       Client.cpp \
       Channel.cpp \
       Command.cpp

SRDIR = src/
NAME = ircserv
DSRC = $(addprefix $(SRDIR), $(SRCS))
ALLC = $(DSRC)
OBJS = $(DSRC:.cpp=.o)
CXX = c++ -Wall -Werror -Wextra  -std=c++98 -I inc
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
