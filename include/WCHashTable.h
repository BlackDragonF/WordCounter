#ifndef WC_HASH_TABLE_H
#define WC_HASH_TABLE_H

/* include necessary libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* include word/index/error modules */
#include "WCWord.h"
#include "WCIndex.h"
#include "WCFileHandler.h"

/* struct declaration */
struct WCHashTable;
typedef struct WCHashTable WCHashTable;

struct WCHashTableIterator;
typedef struct WCHashTableIterator WCHashTableIterator;

/* function list */

/* construct and destruct */
WCHashTable * wc_hash_table_create(int size, WCError * error);
void wc_hash_table_destroy(WCHashTable * hash, WCError * error);

/* insert/search/delete */
void wc_hash_table_insert_word(WCHashTable * hash, WCWord * word, WCError * error);
WCIndex * wc_hash_table_search_word(WCHashTable * hash, WCWord * word, WCError * error);
void wc_hash_table_delete_word(WCHashTable * hash, WCWord * word, WCError * error);

/* get operations */
int wc_hash_table_get_count(WCHashTable * hash, WCError * error);
int wc_hash_table_get_unique(WCHashTable * hash, WCError * error);

/* iterator related */
WCHashTableIterator * wc_hash_table_iterator_create(WCHashTable * hash, WCError * error);
void wc_hash_table_iterator_destroy(WCHashTableIterator * iterator, WCError * error);
void wc_hash_table_iterator_next(WCHashTableIterator * iterator, WCError * error);
WCIndex * wc_hash_table_iterator_get_index(WCHashTableIterator * iterator, char ** word, WCError * error);

/* serialization and deserialization */
void wc_hash_table_write_to_file(WCHashTable * hash, WCFileHandler * handler, const char * path, WCError * error);
WCHashTable * wc_hash_table_read_from_file(WCFileHandler * handler, const char * path, WCError * error);

#endif
