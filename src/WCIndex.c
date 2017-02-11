#include "WCIndex.h"


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
    if (info.row <= 0 || info.column <= 0) {
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

int wc_index_get_count(WCIndex * index, WCError * error) {
    if (index == NULL) {
        *error = WCNullPointerError;
        return 0;
    }
    *error = WCNoneError;
    return index->count;
}

struct WCIndexIterator {
    struct WCIndexNode * iterator;
};

WCIndexIterator * wc_index_iterator_create(WCIndex * index, WCError * error) {
    if (index == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCIndexIterator * iterator = malloc(sizeof(WCIndexIterator));
    if (iterator == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    iterator->iterator = index->head->next;
    if (iterator->iterator == NULL) {
        *error = WCIndexRangeError;
        free(iterator);
        return NULL;
    }
    *error = WCNoneError;
    return iterator;
}

void wc_index_iterator_destroy(WCIndexIterator * iterator, WCError * error) {
    if (iterator == NULL) {
        *error = WCNullPointerError;
        return;
    }
    free(iterator);
    *error = WCNoneError;
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

static void wc_struct_index_node_serialize(WCIndexNode * node, FILE * fp, WCError * error) {
    if (node == NULL || fp == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCObjectTag tag = WCIndexNodeObject;
    fwrite(&tag, sizeof(WCObjectTag), 1, fp);
    fwrite(node, sizeof(WCIndexNode), 1, fp);
    *error = WCNoneError;
}

void wc_struct_index_serialize(WCIndex * index, FILE * fp, WCError * error) {
    if (index == NULL || fp == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCObjectTag tag = WCIndexObject;
    fwrite(&tag, sizeof(WCObjectTag), 1, fp);
    fwrite(index, sizeof(WCIndex), 1, fp);
    // Head node is NOT INCLUDED
    WCIndexNode * temp = index->head->next;
    WCError internalError;
    while (temp) {
        wc_struct_index_node_serialize(temp, fp, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        temp = temp->next;
    }
    *error = WCNoneError;
}

WCIndex * wc_struct_index_deserialize(FILE * fp, WCError * error) {
    if (fp == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCObjectTag tag;
    WCIndex * index = malloc(sizeof(WCIndex));
    if (index == NULL) {
        exit(WCMemoryOverflowError);
    }
    fread(index, sizeof(WCIndex), 1, fp);
    index->head = malloc(sizeof(WCIndexNode));
    if (index->head == NULL) {
        *error = WCMemoryOverflowError;
        free(index);
        return NULL;
    }
    index->head->info.row = 0;
    index->head->info.column = 0;
    index->head->next = NULL;
    WCIndexNode * tail = index->head;
    int count;
    for (count = 0 ; count < index->count ; ++count) {
        fread(&tag, sizeof(WCObjectTag), 1, fp);
        if (tag != WCIndexNodeObject) {
            exit(WCFileInternalError);
        }
        WCIndexNode * temp = malloc(sizeof(WCIndexNode));
        fread(temp, sizeof(WCIndex), 1, fp);
        temp->next = NULL;
        tail->next = temp;
        tail = tail->next;
    }
    *error = WCNoneError;
    return index;
}
