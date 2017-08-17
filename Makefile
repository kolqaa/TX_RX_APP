PROG_NAME = prog

SRC =  	main.c \
	transmitter.c \
	receiver.c \
	md5.c \
	send_recv_func.c \
	file_func.c 

CFLAGS = -lcrypto

CC = gcc

all: comp

comp:
	$(CC) $(SRC) $(CFLAGS) -o $(PROG_NAME)

clean:
	@rm -f $(PROG_NAME)
	@echo "File was clean."

fclean: clean
	@rm -f $(PROG_NAME)
	@echo "File was fclean (fclean rule)."

re: fclean all

trash:
	rm -f *.c~
	rm -f *.h~
