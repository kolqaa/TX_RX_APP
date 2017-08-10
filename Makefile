PROG_NAME = prog

SRC =  	main.c \
	transmitter.c \
	receiver.c \
	md5.c \
	trimm.c 

CFLAGS = -Wall -Wextra -Werror -lcrypto

CC = gcc

all: comp

comp:
	$(CC) $(SRC) $(CFLAGS) -o $(PROG_NAME)

clean:
	@rm -f a.out
	@echo "File was clean."

fclean: clean
	@rm -f a.out
	@echo "File was fclean (fclean rule)."

re: fclean all
