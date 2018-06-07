EXE = compression
SRC = huffman.c \
	HC_error.c \
	HC_read_write.c \
	HC_map_char.c \
	HC_func_comp.c \
	HC_huffman_tree.c \
	HC_mergesort.c \
	HC_priority_queue.c \
	HC_state.c \
	HC_utf8.c \
	HC_prologue.c \
	HC_program.c \
	HC_epilogue.c
OBJ = $(SRC:.c=.o)
CC  = gcc

CFLAGS += -Wall -Wextra -pedantic
CFLAGS += -g
CFLAGS += -fsanitize=address -fno-omit-frame-pointer
#LFLAGS += -lm

vpath %.c src/
vpath %.h src/

.PHONY: all clean distclean

all : $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LFLAGS)

clean:
	rm -f $(OBJ)

distclean: clean
	rm -f compression a.out scratch*

$(OBJ): \
	Makefile \
	HC_mergesort.h \
	HC_huffman_tree.h \
	HC_func_comp.h \
	HC_priority_queue.h \
	HC_struct.h \
	HC_read_write.h \
	HC_utf8.h \
	HC_state.h \
	HC_map_char.h \
	HC_error.h \
	HC_prologue.h \
	HC_epilogue.h
