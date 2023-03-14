CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g3 -I./
LIB = -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
HEADER = blockchain.h
BETTY = betty

SRC0 = blockchain_create.c test/blockchain_create-main.c provided/_blockchain_print.c provided/_blockchain_destroy.c
OBJ0 = $(patsubst %.c, obj/%.o, $(SRC0))
NAME0 = bin/blockchain_create-test

SRC1 = blockchain_create.c block_create.c test/block_create-main.c provided/_blockchain_print.c provided/_blockchain_destroy.c
OBJ1 = $(patsubst %.c, obj/%.o, $(SRC1))
NAME1 = block_create-test

SRC2 =
OBJ2 = $(patsubst %.c, obj/%.o, $(SRC2))
NAME2 =

SRC3 =
OBJ3 = $(patsubst %.c, obj/%.o, $(SRC3))
NAME3 =

SRC4 =
OBJ4 = $(patsubst %.c, obj/%.o, $(SRC4))
NAME4 =

SRC5 =
OBJ5 = $(patsubst %.c, obj/%.o, $(SRC5))
NAME5 =

SRC6 =
OBJ6 = $(patsubst %.c, obj/%.o, $(SRC6))
NAME6 =

SRC7 =
OBJ7 = $(patsubst %.c, obj/%.o, $(SRC7))
NAME7 =

ALLOBJ = $(OBJ0) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7)
ALLNAME = $(NAME0) $(NAME1) $(NAME2) $(NAME3) $(NAME4) $(NAME5) $(NAME6) $(NAME7)

.PHONY: directories task0 task1 task2 task3 task4 task5 task6 task7

all: directories task0 task1 task2 task3 task4 task5 task6 task7

directories:
	mkdir -p obj bin obj/test obj/provided

obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Task 0-7
task0: directories $(HEADER) $(OBJ0)
	$(CC) $(CFLAGS) $(OBJ0) -o $(NAME0) $(LIB)
	./$(NAME0)

task1: directories $(OBJ1)
	$(CC) $(CFLAGS) $(OBJ1) -o $(NAME1) $(LIB)
	./$(NAME1)

task2: directories $(HEADER) $(OBJ2)
	$(CC) $(CFLAGS) $(OBJ2) -o $(NAME2) $(LIB)
	./$(NAME2)

task3: directories $(HEADER) $(OBJ3)
	$(CC) $(CFLAGS) $(OBJ3) -o $(NAME3) $(LIB)
	./$(NAME3)

task4: directories $(HEADER) $(OBJ4)
	$(CC) $(CFLAGS) $(OBJ4) -o $(NAME4) $(LIB)
	./$(NAME4)

task5: directories $(HEADER) $(OBJ5)
	$(CC) $(CFLAGS) $(OBJ5) -o $(NAME5) $(LIB)
	./$(NAME5)

task6: directories $(HEADER) $(OBJ6)
	$(CC) $(CFLAGS) $(OBJ6) -o $(NAME6) $(LIB)
	./$(NAME6)

task7: directories $(HEADER) $(OBJ7)
	$(CC) $(CFLAGS) $(OBJ7) -o $(NAME7) $(LIB)
	./$(NAME7)

clean:
	$(foreach var, $(ALLNAME), $(RM) *~ $(var))

oclean:
	$(foreach var, $(ALLOBJ), $(RM) $(var))

fclean: clean oclean

re: oclean all

betty:
	$(BETTY) *.c **/**.c *.h Makefile
