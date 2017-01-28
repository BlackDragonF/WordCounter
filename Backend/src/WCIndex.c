#include "WCIndex.h"
#include "WCErrorHandler.h"

struct WCIndexNode {
    WCWordInfo info;
    struct WCIndexNode * next;
};
typedef struct WCIndexNode WCIndexNode;

struct WCIndex {
    int count;
    struct WCIndexNode * head;
};

WCIndex * wc_index_create(WCError * error) {
    WCIndex * index = malloc(sizeof(WCIndex));
    if (index == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    index->count = 0;
    index->head = malloc(sizeof(WCIndexNode));
    if (index->head == NULL) {
        *error = WCMemoryOverflowError;
        free(index);
        return NULL;
    }
    index->head->info.row = 0;
    index->head->info.column = 0;
    index->head->next = NULL;
    *error = WCNoneError;
    return index;
}

void wc_index_destroy(WCIndex * index, WCError * error) {
    if (index == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCIndexNode * p = index->head;
    WCIndexNode * temp;
    while (p) {
        temp = p;
        p = p->next;
        free(temp);
    }
    free(index);
    *error = WCNoneError;
}

void wc_index_add(WCIndex * index, WCWordInfo info, WCError * error) {
    if (index == NULL) {
        *error = WCNullPointerError;
        return;
    }
    if (info.line <= 0 || info.position <= 0) {
        *error = WCIndexRangeError;
        return;
    }
    WCIndexNode * temp = malloc(sizeof(WCIndexNode));
    if (temp == NULL) {
        *error = WCMemoryOverflowError;
        return;
    }
    temp->info = info;
    temp->next = index->head->next;
    index->head->next = temp;
    index->count++;
    *error = WCNoneError;
}

struct WCIndexIterator {
    WCIndeNode * iterator;
};

WCIndexIterator wc_index_iterator_create(WCIndex * index, WCError * error) {
    WCIndexIterator iterator;
    if (index == NULL) {
        *error = WCNullPointerError;
        return iterator;
    }
    iterator.iterator = index->head->next;
    if (iterator.iterator == NULL) {
        *error = WCIndexRangeError;
    }
    *error = WCNoneError;
    return iterator;
}

void wc_index_iterator_next(WCIndexIterator * iterator, WCError * error) {
    if (iterator == NULL) {
        *error = WCNullPointerError;
        return;
    }
    if (iterator->iterator->next) {
        iterator->iterator = iterator->iterator->next;
        *error = WCNoneError;
    } else {
        *error = WCIndexRangeError;
    }
}

WCWordInfo wc_index_iterator_get_value(WCIndexIterator * iterator, WCError * error) {
    if (iterator == NULL) {
        *error = WCNoneError;
        WCWordInfo info;
        info.row = 0;
        info.column = 0;
        return info;
    }
    *error = WCNoneError;
    return iterator->iterator->info;
}
