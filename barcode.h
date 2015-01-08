#ifndef _BARCODE_H_
#define _BARCODE_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "hat-trie.h"

#include "bloom-filter.h"
#include "hash-string.h"
void hattrie_iteration(hattrie_t* T, char* m_label, char* output_label);
void make_repeat_and_unique_tries(char* reads_file_path, hattrie_t* trie_unique, hattrie_t* trie_repeat, int* results);
void hash_trie_into_bf(hattrie_t* T, BloomFilter* bf);
void check_if_trie_in_bf(hattrie_t* T, BloomFilter* bf);
void query_bf_with_genome(BloomFilter* bf_unique, FILE* genome_file ,hattrie_t* trie_genome_uniqe, int read_size);
void check_fp(hattrie_t* trie_true, hattrie_t* trie_to_check, hattrie_t* trie_push,  hattrie_t* trie_to_check_true);
void check_fn(hattrie_t* trie_reads, hattrie_t* trie_true_reference, hattrie_t* trie_push);
void print_bf(BloomFilter* bf, int tables_size, int num_of_hash_func, char* label_1, char* label_2);
void encode(hattrie_t* trie_unique, FILE* genome, BloomFilter* bf, hattrie_t* trie_fp, hattrie_t* trie_fn ,int read_size, char* label, int bf_table_size,int num_of_hash_func);
void load_bf(char* bf_path, BloomFilter** bf, int* bf_results);
void load_file_to_trie(char* reads_file_path, hattrie_t* reads_trie);
void decode(char* bf_path, char* repeat_file_path, char* genome_file_path, char* fn_file_path, char* fp_file_path, int read_size, char* label);
#endif
