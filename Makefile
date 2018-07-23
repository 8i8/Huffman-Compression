SRCS = $(wildcard src/*/*.c)
TSRCS = $(wildcard tests/*.c)
OBJ = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -pedantic
CFLAGS += -Werror
CFLAGS += -g
CFLAGS += -fsanitize=address -fno-omit-frame-pointer
CFLAGS += -I./include

#VPATH = src:../include
#vpath $(FOLDER)

#vpath %.c src/
#vpath %.h include/

.PHONY: all clean distclean

all : compress

compress: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	$(RM) $(OBJ)

distclean: clean
	rm -f compression a.out scratch*

$(OBJ): Makefile

