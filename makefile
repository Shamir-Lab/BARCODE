CC = gcc
INCLUDE = -I/specific/a/nas-eran-01/groups/eran_halperin/yaronein/projects/BARCODE/scripts/include/hat-trie
LIBS = -lm
OBJECTS = test_barcode.c barcode.c bloom-filter.c hash-string.c pstdint.h common.h ahtable.h ahtable.c hat-trie.h hat-trie.c misc.h misc.c murmurhash3.h murmurhash3.c
all: 
	$(CC) -o test_barcode $(OBJECTS) $(LIBS)
