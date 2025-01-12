NAME	=	minitalk

CC		=	cc
CFLAGS	=	-Wall -Werror -Wextra
RM		=	rm -f

CLIENT	=	client
SERVER	=	server

all:		$(NAME)

$(SERVER):	$(SERVER).c minitalk.h
			@echo "Make all\nCompiling the server."
			@$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).c

$(CLIENT):	$(CLIENT).c minitalk.h
			@echo "Compiling the Client."
			@$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).c

$(NAME):	$(SERVER) $(CLIENT)

clean:
			@echo "Make clean/fclean\nRemoving all compiled files."
			@$(RM) $(CLIENT) $(SERVER)

fclean:		clean

re:			fclean all

.PHONY: 	all clean fclean re