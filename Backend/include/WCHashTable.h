#ifndef WC_HASH_TABLE_H
#define WC_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WCWord.h"
#include "WCIndex.h"

struct WCHashTable;
typedef struct WCHashTable WCHashTable;

struct WCHashTableIterator;
typedef struct WCHashTableIterator WCHashTableIterator;

WCHashTable * wc_hash_table_create(int size, WCError * error);
void wc_hash_table_destroy(WCHashTable * hash, WCError * error);
void wc_hash_table_insert_word(WCHashTable * hash, WCWord * word, WCError * error);
WCIndex * wc_hash_table_search_word(WCHashTable * hash, WCWord * word, WCError * error);
void wc_hash_table_delete_word(WCHashTable * hash, WCWord * word, WCError * error);

WCHashTableIterator * wc_hash_table_iterator_create(WCHashTable * hash, WCError * error);
void wc_hash_table_iterator_destroy(WCHashTableIterator * iterator, WCError * error);
void wc_hash_table_iterator_next(WCHashTableIterator * iterator, WCError * error);
WCIndex * wc_hash_table_iterator_get_index(WCHashTableIterator * iterator, char ** word, WCError * error);

#endif
