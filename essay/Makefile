NAME = WebServ

SRCS = main.cpp Response.cpp

CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

all:	$(NAME)

$(NAME):	$(SRCS)
				$(CPP) $(CPPFLAGS) $(SRCS) -o $(NAME)

fclean: 	clean
			rm -f $(NAME)

re: 		fclean all

.PHONY: 	all re clean fclean