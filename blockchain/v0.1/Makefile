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
NAME1 = bin/block_create-test

SRC2 = blockchain_create.c block_create.c block_destroy.c test/block_destroy-main.c provided/_blockchain_destroy.c
OBJ2 = $(patsubst %.c, obj/%.o, $(SRC2))
NAME2 = bin/block_destroy-test

SRC3 = blockchain_destroy.c blockchain_create.c block_create.c block_destroy.c test/blockchain_destroy-main.c
OBJ3 = $(patsubst %.c, obj/%.o, $(SRC3))
NAME3 = bin/blockchain_destroy-test

SRC4 = blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c test/block_hash-main.c provided/_blockchain_destroy.c provided/_blockchain_print.c
OBJ4 = $(patsubst %.c, obj/%.o, $(SRC4))
NAME4 = bin/block_hash-test

SRC5 = blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c blockchain_serialize.c provided/_endianness.c test/blockchain_serialize-main.c
OBJ5 = $(patsubst %.c, obj/%.o, $(SRC5))
NAME5 = bin/blockchain_serialize-test

SRC6 = blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c blockchain_deserialize.c provided/_blockchain_print.c provided/_endianness.c test/blockchain_deserialize-main.c
OBJ6 = $(patsubst %.c, obj/%.o, $(SRC6))
NAME6 = bin/blockchain_deserialize-test

SRC7 = blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c test/block_is_valid-main.c
OBJ7 = $(patsubst %.c, obj/%.o, $(SRC7))
NAME7 = bin/block_is_valid-test

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
	valgrind ./$(NAME0)

task1: directories $(OBJ1)
	$(CC) $(CFLAGS) $(OBJ1) -o $(NAME1) $(LIB)
	./$(NAME1)

task2: directories $(HEADER) $(OBJ2)
	$(CC) $(CFLAGS) $(OBJ2) -o $(NAME2) $(LIB)
	valgrind ./$(NAME2)

task3: directories $(HEADER) $(OBJ3)
	$(CC) $(CFLAGS) $(OBJ3) -o $(NAME3) $(LIB)
	valgrind ./$(NAME3)

task4: directories $(HEADER) $(OBJ4)
	$(CC) $(CFLAGS) $(OBJ4) -o $(NAME4) $(LIB)
	./$(NAME4)

task5: directories $(HEADER) $(OBJ5)
	$(CC) $(CFLAGS) $(OBJ5) -o $(NAME5) $(LIB)
	./$(NAME5)
	hexdump -Cv save.hblk

task6: directories $(HEADER) $(OBJ6)
	$(CC) $(CFLAGS) $(OBJ6) -o $(NAME6) $(LIB)
	./$(NAME6)

task7: directories $(HEADER) $(OBJ7)
	$(CC) $(CFLAGS) $(OBJ7) -o $(NAME7) $(LIB)
	./$(NAME7)

clean:
	$(foreach var, $(ALLNAME), $(RM) *~ $(var))
	$(RM) save.hblk

oclean:
	$(foreach var, $(ALLOBJ), $(RM) $(var))

fclean: clean oclean

re: oclean all

betty:
	$(BETTY) *.c **/**.c *.h Makefile
