#ifndef WC_HASH_TABLE_H
#define WC_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WCWord.h"
#include "WCIndex.h"

struct WCHashTable;
typedef struct WCHashTable WCHashTable;

WCHashTable * wc_hash_table_create(int size, WCError * error);
void wc_hash_table_destroy(WCHashTable * hash, WCError * error);
void wc_hash_table_insert_word(WCHashTable * hash, WCWord * word, WCError * error);
WCIndex * wc_hash_table_search_word(WCHashTable * hash, WCWord * word, WCError * error);

#endif
