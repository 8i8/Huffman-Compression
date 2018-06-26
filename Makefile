EXE = compression
CFLAGS = -Wall -Wextra -Isrc
SRCS = $(wildcard src/*.c)
TSRCS = $(wildcard tests/*.c)
OBJ = $(SRCS:.c=.o)

CFLAGS += -Wall -Wextra -pedantic
#CFLAGS += -Werror
CFLAGS += -g
CFLAGS += -fsanitize=address -fno-omit-frame-pointer
#LFLAGS += -lm

vpath %.c src/
vpath %.h src/

.PHONY: all clean distclean

all :
	$(MAKE) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LFLAGS)

clean:
	$(RM) $(OBJ)

distclean: clean
	rm -f compression a.out scratch*

