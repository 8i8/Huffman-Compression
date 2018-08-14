SRC = $(wildcard *.c)
HDR = $(wildcard ../../include/*/*.c)
OBJ = $(SRC:.c=.o)
CC  = gcc

CFLAGS += -Wall -Wextra -pedantic
#CFLAGS += -Werror
CFLAGS += -g
CFLAGS += -fsanitize=address
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -fsanitize=undefined
CFLAGS += -fsanitize=float-divide-by-zero
CFLAGS += -fno-sanitize-recover
CFLAGS += -I../../include
LFLAGS += -lm

.PHONY: clean cleanall

obs: $(OBJ)
	gcc -c $(CFLAGS) $^ -static

libbitwise: $(OBJ)
	ar -cvq $@ $^

clean:
	$(RM) $(OBJ)

cleanall: clean
	$(RM) lab

$(OBJ): Makefile $(HDR)
