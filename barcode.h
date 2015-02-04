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
#define MEM_CHECK 0
#define TABLE_FACTOR 10
#define HASH_FUNC_FACTOR 0.69314
#define MAX_CASCADES 100
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
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

char* copy_file_to_mem(char* file_path);
int zip(char* file_path, char* archive_prefix_name, char* file_name);
int unzip(char* archive_prefix_name, char* file_name);
int zip_encoded_files(char* archive_prefix_name);
int unzip_encoded_files(char* archive_prefix_name);
int hattrie_iteration(hattrie_t* T, char* m_label, char* output_label, int with_duplicates);
int make_repeat_and_unique_tries(char* reads_file_path, hattrie_t* trie_unique, hattrie_t* trie_repeat, int* results);
int hash_trie_into_bf(hattrie_t* T, BloomFilter* bf);
void check_if_trie_in_bf(hattrie_t* T, BloomFilter* bf);
int query_bf_with_genome(BloomFilter* bf_unique, FILE* genome_file ,hattrie_t* trie_genome_uniqe, int read_size);
void check_fp(hattrie_t* trie_true, hattrie_t* trie_to_check, hattrie_t* trie_push,  hattrie_t* trie_to_check_true);
void check_fn(hattrie_t* trie_reads, hattrie_t* trie_true_reference, hattrie_t* trie_push);
int print_bf(BloomFilter* bf, long long tables_size, int num_of_hash_func, char* label_1, char* label_2);
int encode(hattrie_t* trie_unique, FILE* genome, BloomFilter* bf, int read_size, char* label, long long bf_table_size,int num_of_hash_func, long long* number_of_fp_reads);
int load_bf(char* bf_path, BloomFilter** bf, int* bf_results);
int load_file_to_trie(char* reads_file_path, hattrie_t* reads_trie);
void make_path(char* path, char* directory, char* label_1, char* label_2);
void encode_file(char * read_file_path, char* genome_file_path, char* label, int with_zip, int with_cascade);
int decode(char* repeat_file_path, char* genome_file_path, char* fn_file_path, char* fp_file_path, int read_size, char* label, int cascade_number);
void decode_file(char* genome_file_path, char* label, int with_zip, int with_cascade);
#endif
