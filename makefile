SHELL = /bin/sh
CC = gcc
EXEC = barcode
INCLUDE = -I/usr/include/python2.7
SRC = $(EXEC).c test_barcode.c bloom-filter.c hash-string.c ahtable.c hat-trie.c misc.c murmurhash3.c
OBJ = $(SRC:.c=.o)
LIB = pstdint.h common.h ahtable.h hat-trie.h misc.h murmurhash3.h 
all: $(EXEC)

clean:
	rm *.o barcode

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ -lm

%.o: %.c $(LIB)
	$(CC) -o $@ -c $<
	
# $(CC) -o barcode  $(INCLUDE) $(OBJECTS) $(LIBS)


# SHELL = /bin/sh
# CC = gcc
# CFLAGS = -Wall -O3 -funroll-all-loops
# EXEC = program
# SRC = $(EXEC).c file1.c file2.c file3.c file4.c
# OBJ = $(SRC:.c=.o)
# LIB = $(SRC:.c=.h)

# all: $(EXEC)

# $(EXEC): $(OBJ) $(LIB)
#     $(CC) -o $@ $^ $(LDFLAGS) -lm

# %.o: %.c $(LIB)
#     $(CC) -o $@ -c $< $(CFLAGS)