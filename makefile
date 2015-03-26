CC = gcc
INCLUDE = -I/usr/include/python2.7
LIBS = -lm
OBJECTS = test_barcode.c barcode.c bloom-filter.c hash-string.c pstdint.h common.h ahtable.h ahtable.c hat-trie.h hat-trie.c misc.h misc.c murmurhash3.h murmurhash3.c
all: 
	$(CC) -o barcode  $(INCLUDE) $(OBJECTS) $(LIBS)
