EXE = compression
SRC = huffman.c \
	  HC_frequency_map.c \
	  HC_HuffmanTree.c \
	  HC_ListSort.c \
	  HC_CharMap.c \
	  HC_Error.c \
	  HC_Message.c \
	  hc_msg.c
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
	HC_frequency_map.h \
	HC_HuffmanTree.h \
	HC_CharMap.h \
	HC_Error.h \
	HC_Message.h \
	hc_msg.h
