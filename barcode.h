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
int check_legal_char(char c , int line_number);
void handle_line(char *line, hattrie_t* trie_unique, hattrie_t* trie_repeat, int has_N);
void hattrie_iteration(hattrie_t* T, char* m_label, char* output_label);
void make_repeat_and_unique_tries(char* reads_file_path, hattrie_t* trie_unique, hattrie_t* trie_repeat);
void hash_trie_into_bf(hattrie_t* T, BloomFilter* bf);
void check_if_trie_in_bf(hattrie_t* T, BloomFilter* bf);
void query_bf_with_genome(BloomFilter* bf_unique, FILE* genome_file ,hattrie_t* trie_genome_uniqe, int read_size);
void check_fp(hattrie_t* trie_true, hattrie_t* trie_to_check, hattrie_t* trie_push,  hattrie_t* trie_to_check_true);
void check_fn(hattrie_t* trie_reads, hattrie_t* trie_true_reference, hattrie_t* trie_push);
void print_bf(BloomFilter* bf, int tables_size, char* label_1, char* label_2);

#endif
