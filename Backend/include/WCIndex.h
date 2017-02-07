#ifndef WC_INDEX_H
#define WC_INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WCErrorHandler.h"

struct WCIndex;
typedef struct WCIndex WCIndex;

struct WCIndexIterator;
typedef struct WCIndexIterator WCIndexIterator;

struct WCWordInfo {
    int row;
    int column;
};
typedef struct WCWordInfo WCWordInfo;

WCIndex * wc_index_create(WCError * error);
void wc_index_destroy(WCIndex * index, WCError * error);
void wc_index_add(WCIndex * index, WCWordInfo info, WCError * error);
int wc_index_get_count(WCIndex * index);

WCIndexIterator * wc_index_iterator_create(WCIndex * index, WCError * error);
void wc_index_iterator_next(WCIndexIterator * iterator, WCError * error);
WCWordInfo wc_index_iterator_get_value(WCIndexIterator * iterator, WCError * error);

#endif
