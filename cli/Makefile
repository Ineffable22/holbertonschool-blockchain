CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g3
BETTY = betty

VERSION = v0.3
HDR_CRYPTO = ../crypto/
HDR_BLOCK = ../blockchain/$(VERSION)/
HDR_TRANSACTION = ../blockchain/$(VERSION)/transaction/
HDR_PROVIDED = ../blockchain/$(VERSION)/provided/

LINKLIBS = hblk_blockchain hblk_crypto llist ssl crypto readline
MULTITHREAD = -pthread
LDLIBS = $(addprefix -l, $(LINKLIBS)) $(MULTITHREAD)

LIBRARY = $(addprefix -I, $(HDR_CRYPTO) $(HDR_BLOCK) $(HDR_TRANSACTION) $(HDR_PROVIDED))
LIBDIR  = $(addprefix -L, $(HDR_CRYPTO) $(HDR_BLOCK) $(HDR_TRANSACTION) $(HDR_PROVIDED))
LDFLAGS = $(LIBRARY) $(LIBDIR)

HEADER = cli.h ../crypto/hblk_crypto.h ../blockchain/$(VERSION)/blockchain.h ../blockchain/$(VERSION)/transaction/transaction.h ../blockchain/$(VERSION)/provided/endianness.h
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, obj/%.o, $(SRC))
NAME = bin/hblk_cli

LIBTEST = -L./lib -lunity
SRCTEST = $(filter-out cli.c, $(wildcard *.c)) test/test-send.c
OBJTEST = $(patsubst %.c, obj/%.o, $(SRCTEST))

.PHONY: directories all clean fclean re cli

all: directories cli

directories:
	mkdir -p obj bin
	$(MAKE) -C ../crypto libhblk_crypto.a
	$(MAKE) -C ../blockchain/$(VERSION) libhblk_blockchain.a

obj/%.o: %.c
	$(CC) $(CFLAGS) $(LDLIBS) -c $< -o $@

cli: directories $(HEADER) $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME) $(LDLIBS)
	clear
	./$(NAME)

test: directories $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCTEST) -o $(NAME) $(LDLIBS) $(LIBTEST)
	clear
	./$(NAME)

clean:
	$(foreach var, $(OBJ), $(RM) *~ $(var))

oclean:
	$(foreach var, $(ALLOBJ), $(RM) $(var))

fclean: clean oclean
	$(MAKE) -C ../crypto fclean
	$(MAKE) -C ../blockchain/$(VERSION) fclean



re: oclean all

betty:
	$(BETTY) *.c **/**.c *.h Makefile
