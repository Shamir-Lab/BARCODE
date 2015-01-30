/*
 * This file is part of hat-trie.
 *
 * Copyright (c) 2011 by Daniel C. Jones <dcjones@cs.washington.edu>
 *
 * misc :
 * miscelaneous functions.
 *
 */

#ifndef LINESET_MISC_H
#define LINESET_MISC_H

#include <stdio.h>

void* malloc_or_die(long long);
void* realloc_or_die(void*, long long);
FILE* fopen_or_die(const char*, const char*);

#endif


