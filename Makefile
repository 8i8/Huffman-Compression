SRCS = $(wildcard src/*/*.c)
HEADR = $(wildcard include/*/*.h)
TSRCS = $(wildcard tests/*.c)
OBJ = $(SRCS:.c=.o)

CFLAGS += -Wall -Wextra -pedantic
#CFLAGS += -Werror
CFLAGS += -g
CFLAGS += -fsanitize=address -fno-omit-frame-pointer
CFLAGS += -I./include

.PHONY: clean distclean

compress: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(OBJ)

distclean: clean
	rm -f compress a.out scratch*

$(OBJ): Makefile $(HEADR)

