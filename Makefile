EXE = compression
SRC = huffman.c \
	HC_char_map.c \
	HC_comp_functions.c \
	HC_Error.c \
	HC_Huffman_tree.c \
	HC_mergesort.c \
	HC_print_output.c \
	HC_priority_queue.c \
	HC_utf8.c \
	HC_binary.c
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
	HC_char_map.h \
	HC_comp_functions.h \
	HC_Error.h \
	HC_Huffman_tree.h \
	HC_mergesort.h \
	HC_priority_queue.h \
	HC_Struct.h \
	HC_utf8.h \
	HC_binary.h
