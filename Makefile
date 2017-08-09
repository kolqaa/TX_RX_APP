SRC =  main.c \
	transmitter.c \
	receiver.c

all: comp

comp:
	gcc $(SRC) -o prog

clean:
	@rm -f a.out
	@echo "Object files of library has been deleted (clean rule)."

fclean: clean
	@rm -f a.out
	@echo "Library has been deleted (fclean rule)."

re: fclean all
