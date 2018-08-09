SRCS = $(wildcard src/*/*.c)
HEADR = $(wildcard include/*/*.h)
TSRCS = $(wildcard tests/*.c)
OBJ = $(SRCS:.c=.o)
CC = gcc

CFLAGS += -Wall -Wextra -pedantic
#CFLAGS += -Werror
CFLAGS += -g
CFLAGS += -fsanitize=address
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -fsanitize=undefined
CFLAGS += -fsanitize=float-divide-by-zero
CFLAGS += -fno-sanitize-recover
CFLAGS += -I./include

.PHONY: clean distclean

ifndef VERBOSE
.SILENT:
endif

tartar: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(OBJ)

distclean: clean
	rm -f compress a.out scratch*

$(OBJ): Makefile

