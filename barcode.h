#ifndef _BARCODE_H_
#define _BARCODE_H
#if defined(__GNUC__)
  // Ensure we get the 64-bit variants of the CRT's file I/O calls (TAKEN FROM MINIZ EXAMPLES - yeinhorn)
  #ifndef _FILE_OFFSET_BITS
    #define _FILE_OFFSET_BITS 64
  #endif
  #ifndef _LARGEFILE64_SOURCE
    #define _LARGEFILE64_SOURCE 1
  #endif
#endif
//if MEM_CHECK==1, in non-verbose mode it will give current memeory usage as well. otherwise set MEM_CHECK=0
#define MEM_CHECK 0
#define TABLE_FACTOR 10
#define HASH_FUNC_FACTOR 0.69314
#define MAX_CASCADES 100
#define PYTHON "python"
#define SCALCE "./scalce"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>
#include "hat-trie.h"

#include "bloom-filter.h"
#include "hash-string.h"

extern int VERBOSE_MODE;
extern int KEEP_FILES;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;


void make_path(char* path, char* directory, char* label_1, char* label_2);
void encode_file(char * read_file_path, char* genome_file_path, char* label, int with_zip, int with_cascade);
void decode_file(char* genome_file_path, char* label, int with_zip, int with_cascade);
#endif
