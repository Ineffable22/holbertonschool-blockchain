CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto
LIB = -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
HEADER = blockchain.h transaction/transaction.h provided/endianness.h
BETTY = betty

ALLSRC = $(wildcard *.c) $(wildcard transaction/*.c) $(wildcard provided/*.c)
ALLOBJ = $(patsubst %.c, obj/%.o, $(ALLSRC))

ALLSRCLIB = $(wildcard *.c) $(wildcard transaction/*.c)
ALLOBJLIB = $(patsubst %.c, obj/%.o, $(ALLSRCLIB))

SRC0 = test/blockchain_create-main.c $(ALLSRC)
OBJ0 = $(patsubst %.c, obj/%.o, $(SRC0))
NAME0 = bin/blockchain_create-test

SRC1 = test/block_create-main.c $(ALLSRC)
OBJ1 = $(patsubst %.c, obj/%.o, $(SRC1))
NAME1 = bin/block_create-test

SRC2 = test/block_destroy-main.c $(ALLSRC)
OBJ2 = $(patsubst %.c, obj/%.o, $(SRC2))
NAME2 = bin/block_destroy-test

SRC3 = test/blockchain_destroy-main.c $(ALLSRC)
OBJ3 = $(patsubst %.c, obj/%.o, $(SRC3))
NAME3 = bin/blockchain_destroy-test

SRC4 = test/block_hash-main.c $(ALLSRC)
OBJ4 = $(patsubst %.c, obj/%.o, $(SRC4))
NAME4 = bin/block_hash-test

SRC5 = test/blockchain_serialize-main.c $(ALLSRC)
OBJ5 = $(patsubst %.c, obj/%.o, $(SRC5))
NAME5 = bin/blockchain_serialize-test

SRC6 = test/blockchain_deserialize-main.c $(ALLSRC)
OBJ6 = $(patsubst %.c, obj/%.o, $(SRC6))
NAME6 = bin/blockchain_deserialize-test

SRC7 = test/block_is_valid-main.c $(ALLSRC)
OBJ7 = $(patsubst %.c, obj/%.o, $(SRC7))
NAME7 = bin/block_is_valid-test

SRC8 = hash_matches_difficulty.c test/hash_matches_difficulty-main.c
OBJ8 = $(patsubst %.c, obj/%.o, $(SRC8))
NAME8 = bin/hash_matches_difficulty-test

SRC9 = test/block_is_valid-main.c $(ALLSRC)
OBJ9 = $(patsubst %.c, obj/%.o, $(SRC9))
NAME9 = bin/block_is_valid-test2

SRC10 = test/block_mine-main.c $(ALLSRC)
OBJ10 = $(patsubst %.c, obj/%.o, $(SRC10))
NAME10 = bin/block_mine-test

SRC11 = test/blockchain_difficulty-main.c $(ALLSRC)
OBJ11 = $(patsubst %.c, obj/%.o, $(SRC11))
NAME11 = bin/blockchain_difficulty-test

SRC12 = transaction/tx_out_create.c provided/_print_hex_buffer.c transaction/test/tx_out_create-main.c
OBJ12 = $(patsubst %.c, obj/%.o, $(SRC12))
NAME12 = bin/transaction/tx_out_create-test

SRC13 = transaction/tx_out_create.c transaction/unspent_tx_out_create.c provided/_print_hex_buffer.c transaction/test/unspent_tx_out_create-main.c
OBJ13 = $(patsubst %.c, obj/%.o, $(SRC13))
NAME13 = bin/transaction/unspent_tx_out_create-test

SRC14 = transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c provided/_print_hex_buffer.c transaction/test/tx_in_create-main.c
OBJ14 = $(patsubst %.c, obj/%.o, $(SRC14))
NAME14 = bin/transaction/tx_in_create-test

SRC15 = transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c provided/_print_hex_buffer.c transaction/test/transaction_hash-main.c provided/_transaction_print.c
OBJ15 = $(patsubst %.c, obj/%.o, $(SRC15))
NAME15 = bin/transaction/transaction_hash-test

SRC16 = transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c provided/_print_hex_buffer.c transaction/test/tx_in_sign-main.c provided/_transaction_print.c
OBJ16 = $(patsubst %.c, obj/%.o, $(SRC16))
NAME16 = bin/transaction/tx_in_sign-test

SRC17 = transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c provided/_print_hex_buffer.c provided/_transaction_print.c transaction/test/transaction_create-main.c
OBJ17 = $(patsubst %.c, obj/%.o, $(SRC17))
NAME17 = bin/transaction/transaction_create-test

SRC18 = transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_is_valid.c provided/_print_hex_buffer.c transaction/test/transaction_is_valid-main.c provided/_transaction_print.c
OBJ18 = $(patsubst %.c, obj/%.o, $(SRC18))
NAME18 = bin/transaction/transaction_is_valid-test

SRC19 = transaction/tx_out_create.c transaction/transaction_hash.c transaction/coinbase_create.c provided/_print_hex_buffer.c transaction/test/coinbase_create-main.c provided/_transaction_print.c
OBJ19 = $(patsubst %.c, obj/%.o, $(SRC19))
NAME19 = bin/transaction/coinbase_create-test

SRC20 = transaction/tx_out_create.c transaction/transaction_hash.c transaction/coinbase_create.c transaction/coinbase_is_valid.c provided/_print_hex_buffer.c transaction/test/coinbase_is_valid-main.c provided/_transaction_print.c
OBJ20 = $(patsubst %.c, obj/%.o, $(SRC20))
NAME20 = bin/transaction/coinbase_is_valid-test

SRC21 = transaction/tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/coinbase_create.c transaction/transaction_destroy.c transaction/test/transaction_destroy-main.c
OBJ21 = $(patsubst %.c, obj/%.o, $(SRC21))
NAME21 = bin/transaction/transaction_destroy-test

SRC22 = blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c transaction/tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/coinbase_create.c transaction/transaction_destroy.c provided/_genesis.c provided/_print_hex_buffer.c provided/_blockchain_print.c provided/_transaction_print.c provided/_transaction_print_brief.c test/block_hash-main.c
OBJ22 = $(patsubst %.c, obj/%.o, $(SRC22))
NAME22 = bin/block_hash-test

SRC23 = blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c hash_matches_difficulty.c blockchain_difficulty.c block_mine.c transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/transaction_is_valid.c transaction/coinbase_create.c transaction/coinbase_is_valid.c transaction/transaction_destroy.c provided/_genesis.c provided/_print_hex_buffer.c test/block_is_valid-main.c
OBJ23 = $(patsubst %.c, obj/%.o, $(SRC23))
NAME23 = bin/block_is_valid-test

SRC24 = blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c hash_matches_difficulty.c blockchain_difficulty.c block_mine.c transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/transaction_is_valid.c transaction/coinbase_create.c transaction/coinbase_is_valid.c transaction/transaction_destroy.c transaction/update_unspent.c provided/_genesis.c provided/_print_hex_buffer.c provided/_blockchain_print.c provided/_transaction_print.c provided/_transaction_print_brief.c transaction/test/update_unspent-main.c
OBJ24 = $(patsubst %.c, obj/%.o, $(SRC24))
NAME24 = bin/transaction/update_unspent-test

ALLNAME = $(NAME0) $(NAME1) $(NAME2) $(NAME3) $(NAME4) $(NAME5) $(NAME6) $(NAME7) $(NAME8) $(NAME9) $(NAME10) $(NAME11) $(NAME12) $(NAME13) $(NAME14) $(NAME15) $(NAME16) $(NAME17) $(NAME18) $(NAME19) $(NAME20) $(NAME21) $(NAME22) $(NAME23) $(NAME24)
LIBNAME = libhblk_blockchain.a

.PHONY: directories all clean fclean re task0 task1 task2 task3 task4 task5 task6 task7 task8 task9 task10 task11 task12 task13 task14 task15 task16 task17 task18 task19 task20 task21 task22 task23 task24 libhblk_blockchain.a

all: directories task0 task1 task2 task3 task4 task5 task6 task7 task8 task9 task10 task11 task12 task13 task14 task15 task16 task17 task18 task19 task20 task21 task22 task23 task24

directories:
	mkdir -p obj bin bin/transaction obj/test obj/provided obj/transaction obj/transaction/test

obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# V0.1
# Task 0-7
task0: directories $(HEADER) $(OBJ0)
	$(CC) $(CFLAGS) $(OBJ0) -o $(NAME0) $(LIB)
	valgrind ./$(NAME0)

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
	valgrind ./$(NAME4)

task5: directories $(HEADER) $(OBJ5)
	$(CC) $(CFLAGS) $(OBJ5) -o $(NAME5) $(LIB)
	valgrind ./$(NAME5)
	hexdump -Cv save.hblk

task6: directories $(HEADER) $(OBJ6)
	$(CC) $(CFLAGS) $(OBJ6) -o $(NAME6) $(LIB)
	valgrind ./$(NAME6)

task7: directories $(HEADER) $(OBJ7)
	$(CC) $(CFLAGS) $(OBJ7) -o $(NAME7) $(LIB)
	valgrind ./$(NAME7)

# V0.2
# Task 8-11
task8: directories $(HEADER) $(OBJ8)
	$(CC) $(CFLAGS) $(OBJ8) -o $(NAME8) $(LIB)
	valgrind ./$(NAME8)

task9: directories $(HEADER) $(OBJ9)
	$(CC) $(CFLAGS) $(OBJ9) -o $(NAME9) $(LIB)
	valgrind ./$(NAME9)

task10: directories $(HEADER) $(OBJ10)
	$(CC) $(CFLAGS) $(OBJ10) -o $(NAME10) $(LIB)
	valgrind ./$(NAME10)

task11: directories $(HEADER) $(OBJ11)
	$(CC) $(CFLAGS) $(OBJ11) -o $(NAME11) $(LIB)
	valgrind ./$(NAME11)

# V0.3
# Task 12-24
task12: directories $(HEADER) $(OBJ12)
	$(CC) $(CFLAGS) $(OBJ12) -o $(NAME12) $(LIB)
	valgrind ./$(NAME12)

task13: directories $(HEADER) $(OBJ13)
	$(CC) $(CFLAGS) $(OBJ13) -o $(NAME13) $(LIB)
	valgrind ./$(NAME13)

task14: directories $(HEADER) $(OBJ14)
	$(CC) $(CFLAGS) $(OBJ14) -o $(NAME14) $(LIB)
	valgrind ./$(NAME14)

task15: directories $(HEADER) $(OBJ15)
	$(CC) $(CFLAGS) $(OBJ15) -o $(NAME15) $(LIB)
	valgrind ./$(NAME15)

task16: directories $(HEADER) $(OBJ16)
	$(CC) $(CFLAGS) $(OBJ16) -o $(NAME16) $(LIB)
	valgrind -s --leak-check=full ./$(NAME16)

task17: directories $(HEADER) $(OBJ17)
	$(CC) $(CFLAGS) $(OBJ17) -o $(NAME17) $(LIB)
	valgrind ./$(NAME17)

task18: directories $(HEADER) $(OBJ18)
	$(CC) $(CFLAGS) $(OBJ18) -o $(NAME18) $(LIB)
	valgrind ./$(NAME18)

task19: directories $(HEADER) $(OBJ19)
	$(CC) $(CFLAGS) $(OBJ19) -o $(NAME19) $(LIB)
	valgrind ./$(NAME19)

task20: directories $(HEADER) $(OBJ20)
	$(CC) $(CFLAGS) $(OBJ20) -o $(NAME20) $(LIB)
	valgrind ./$(NAME20)

task21: directories $(HEADER) $(OBJ21)
	$(CC) $(CFLAGS) $(OBJ21) -o $(NAME21) $(LIB)
	valgrind ./$(NAME21)

task22: directories $(HEADER) $(OBJ22)
	$(CC) $(CFLAGS) $(OBJ22) -o $(NAME22) $(LIB)
	valgrind ./$(NAME22)

task23: directories $(HEADER) $(OBJ23)
	$(CC) $(CFLAGS) $(OBJ23) -o $(NAME23) $(LIB)
	valgrind ./$(NAME23)

task24: directories $(HEADER) $(OBJ24)
	$(CC) $(CFLAGS) $(OBJ24) -o $(NAME24) $(LIB)
	valgrind ./$(NAME24)

libhblk_blockchain.a: $(HEADER) $(ALLOBJLIB)
	ar rcs $(LIBNAME) $(ALLOBJLIB)

clean:
	$(foreach var, $(ALLNAME), $(RM) *~ $(var))
	$(RM) save.hblk $(LIBNAME)

oclean:
	$(foreach var, $(ALLOBJ), $(RM) $(var))

fclean: clean oclean

re: oclean all

betty:
	$(BETTY) *.c **/**.c *.h Makefile
