NAME	= mat
LIBDIR	= ../libs/C_maths
LIB	= $(LIBDIR)/libcmaths.a

all: $(NAME)

$(NAME): $(LIB) main.o
	gcc -Wall -Werror -Wextra main.o $(LIB) -o $(NAME)

main.o: main.c
	gcc -Wall -Werror -Wextra -c main.c -I $(LIBDIR)/includes

$(LIB):
	make -C $(LIBDIR)

clean:
	make -C $(LIBDIR) clean
	rm -rf main.o

fclean: clean
	make -C $(LIBDIR) fclean
	rm -rf $(NAME)

re: fclean all
