CC = gcc
INCLUDE = -I/specific/a/nas-eran-01/groups/eran_halperin/yaronein/projects/BARCODE/scripts/include/hat-trie
LIBS = -L./lib -lhat-trie -lm
OBJECTS = test_barcode.c barcode.c bloom-filter.c hash-string.c
all: 
	$(CC) -o test_barcode $(OBJECTS) $(INCLUDE) $(LIBS)
