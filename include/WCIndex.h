#ifndef WC_INDEX_H
#define WC_INDEX_H

/* include necessary libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* include error/file modules */
#include "WCErrorHandler.h"
#include "WCFileHandler.h"

/* struct declaration */
struct WCIndex;
typedef struct WCIndex WCIndex;

struct WCIndexIterator;
typedef struct WCIndexIterator WCIndexIterator;

/* struct definition */
struct WCWordInfo {
    int row;
    int column;
};
typedef struct WCWordInfo WCWordInfo;

/* function list */

/* construct and destruct */
WCIndex * wc_index_create(WCError * error);
void wc_index_destroy(WCIndex * index, WCError * error);

/* add */
void wc_index_add(WCIndex * index, WCWordInfo info, WCError * error);

/* get operations */
int wc_index_get_count(WCIndex * index, WCError * error);

/* iterator related */
WCIndexIterator * wc_index_iterator_create(WCIndex * index, WCError * error);
void wc_index_iterator_destroy(WCIndexIterator * iterator, WCError * error);
void wc_index_iterator_next(WCIndexIterator * iterator, WCError * error);
WCWordInfo wc_index_iterator_get_value(WCIndexIterator * iterator, WCError * error);

/* serialization and deserialization */
void wc_struct_index_serialize(WCIndex * index, FILE * fp, WCError * error);
WCIndex * wc_struct_index_deserialize(FILE * fp, WCError * error);

#endif
