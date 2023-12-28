NAME	=		webserv

CC		= 		c++
CFLAGS	= 		-Wall -Wextra -Werror -std=c++98 -g

SRC		=		main.cpp conf/Lexer.cpp conf/Parser.cpp cgi/cgi.cpp

OBJ		= 		$(SRC:%.cpp=%.o)


all:			$(NAME)

$(NAME):		$(OBJ)
				$(CC) $(CFLAGS) $^ -o $@
%.o: %.cpp
				$(CC) $(CFLAGS) -c $< -o $@

clean:
				rm -rf $(OBJ) 

fclean: 		clean
				rm -rf $(NAME)


re: fclean all

.PHONY: all re clean fclean

test:  re
		make clean && ./webserv 

	