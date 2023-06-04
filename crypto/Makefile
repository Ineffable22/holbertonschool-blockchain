CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g3 -I./
LIB = -lssl -lcrypto
HEADER = hblk_crypto.h
BETTY = betty

SRC0 = test/sha256-main.c provided/_print_hex_buffer.c sha256.c
OBJ0 = $(patsubst %.c, obj/%.o, $(SRC0))
NAME0 = bin/sha256-test

SRC1 = test/ec_create-main.c ec_create.c
OBJ1 = $(patsubst %.c, obj/%.o, $(SRC1))
NAME1 = bin/ec_create-test

SRC2 = test/ec_to_pub-main.c provided/_print_hex_buffer.c ec_to_pub.c ec_create.c
OBJ2 = $(patsubst %.c, obj/%.o, $(SRC2))
NAME2 = bin/ec_to_pub-test

SRC3 = test/ec_from_pub-main.c provided/_print_hex_buffer.c ec_from_pub.c ec_to_pub.c ec_create.c
OBJ3 = $(patsubst %.c, obj/%.o, $(SRC3))
NAME3 = bin/ec_from_pub-test

SRC4 = test/ec_save-main.c ec_save.c ec_create.c ec_to_pub.c provided/_print_hex_buffer.c
OBJ4 = $(patsubst %.c, obj/%.o, $(SRC4))
NAME4 = bin/ec_save-test

SRC5 = test/ec_load-main.c ec_load.c ec_save.c ec_create.c ec_to_pub.c provided/_print_hex_buffer.c
OBJ5 = $(patsubst %.c, obj/%.o, $(SRC5))
NAME5 = bin/ec_load-test

SRC6 = test/ec_sign-main.c provided/_print_hex_buffer.c ec_sign.c ec_create.c
OBJ6 = $(patsubst %.c, obj/%.o, $(SRC6))
NAME6 = bin/ec_sign-test

SRC7 = test/ec_verify-main.c provided/_print_hex_buffer.c ec_verify.c ec_sign.c ec_create.c
OBJ7 = $(patsubst %.c, obj/%.o, $(SRC7))
NAME7 = bin/ec_verify-test

STATICLIB = libhblk_crypto.a
STATICLIBFILES = sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c
STATICLIBOBJS = $(patsubst %.c, obj/%.o, $(STATICLIBFILES))

ALLOBJ = $(OBJ0) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7)
ALLNAME = $(NAME0) $(NAME1) $(NAME2) $(NAME3) $(NAME4) $(NAME5) $(NAME6) $(NAME7) $(STATICLIB)

.PHONY: directories task0 task1 task2 task3 task4 task5 task6 task7 libhblk_crypto.a obj

all: directories task0 task1 task2 task3 task4 task5 task6 task7 libhblk_crypto.a

directories:
	mkdir -p obj bin obj/test obj/provided

obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Task 0-8
task0: directories $(HEADER) $(OBJ0)
	$(CC) $(CFLAGS) $(OBJ0) -o $(NAME0) $(LIB)
	valgrind ./$(NAME0) "Holberton School"

task1: directories $(OBJ1)
	$(CC) $(CFLAGS) $(OBJ1) -o $(NAME1) $(LIB)
	valgrind ./$(NAME1)

task2: directories $(HEADER) $(OBJ2)
	$(CC) $(CFLAGS) $(OBJ2) -o $(NAME2) $(LIB)
	valgrind ./$(NAME2)

task3: directories $(HEADER) $(OBJ3)
	$(CC) $(CFLAGS) $(OBJ3) -o $(NAME3) $(LIB)
	valgrind ./$(NAME3)

task4: directories $(HEADER) $(OBJ4)
	$(CC) $(CFLAGS) $(OBJ4) -o $(NAME4) $(LIB)
	valgrind ./$(NAME4) miguel
	ls -l miguel
	openssl ec -text -noout -in miguel/key.pem
	cat miguel/key.pem
	cat miguel/key_pub.pem
	rm -rf miguel

task5: directories $(HEADER) $(OBJ5)
	$(CC) $(CFLAGS) $(OBJ5) -o $(NAME5) $(LIB)
	valgrind ./$(NAME5) miguel
	rm -rf miguel

task6: directories $(HEADER) $(OBJ6)
	$(CC) $(CFLAGS) $(OBJ6) -o $(NAME6) $(LIB)
	valgrind ./$(NAME6)

task7: directories $(HEADER) $(OBJ7)
	$(CC) $(CFLAGS) $(OBJ7) -o $(NAME7) $(LIB)
	valgrind ./$(NAME7)

# Task 8
libhblk_crypto.a: $(STATICLIBOBJS)
	ar rcs $(STATICLIB) $(STATICLIBOBJS)


clean:
	$(foreach var, $(ALLNAME), $(RM) *~ $(var))

oclean:
	$(foreach var, $(ALLOBJ), $(RM) $(var))

fclean: clean oclean

re: oclean all

betty:
	$(BETTY) *.c **/**.c *.h Makefile
