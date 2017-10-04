NAME = ./bin/exec

LIBFOLDER = ./libs

LIBC = $(LIBFOLDER)/C_data_structures
LIBC_STATIC = $(LIBC)/libcstruct.a

LIB_STATIC = $(LIBC_STATIC)
LIB =

SRCS = $(wildcard ./srcs/*.c)
C_OBJ = $(SRCS:.c=.o)
FLAGS = -Wall -Werror -Wextra 
INC = -I ./includes -I $(LIBC)/includes

all: $(LIB_STATIC) $(NAME)

$(LIB_STATIC):
	- git submodule init
	- git submodule update --remote --merge
	make -C $(LIBC)

$(NAME): $(C_OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(C_OBJ) $(LIB_STATIC) $(LIB)

%.o: %.c ./includes/main.h
	$(CC) $(FLAGS) -o $@ -c $< $(INC)

clean:
	make -C $(LIBC) clean
	rm -rf $(C_OBJ)

fclean: clean
	make -C $(LIBC) fclean
	rm -rf $(NAME)

re: fclean all
