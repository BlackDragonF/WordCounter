#include "WCHashTable.h"

struct WCHashEntry {
    char * word;
    WCIndex * index;
    struct WCHashEntry * next;
};

struct WCHashTable {
    int bound;
    struct WCHashEntry ** base;
};

static unsigned int RSHash(const char * str) {
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;
    while (*str) {
        hash = hash * a + (*str++);
        a *= b;
    }
    return (hash & 0x7FFFFFFF);
}

WCHashTable * wc_hash_table_create(int size, WCError * error) {
    if (size <= 0) {
        *error = WCIndexRangeError;
        return NULL;
    }
    WCHashTable * hash = malloc(sizeof(WCHashTable));
    if (hash == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    hash->bound = size;
    hash->base = malloc(sizeof(struct WCHashEntry *) * size);
    if (hash->base == NULL) {
        *error = WCMemoryOverflowError;
        free(hash);
        return NULL;
    }
    memset(hash->base, 0, sizeof(struct WCHashEntry *) * size);
    *error = WCNoneError;
    return hash;
}

void wc_hash_table_destroy(WCHashTable * hash, WCError * error) {
    if (hash == NULL) {
        *error = WCNullPointerError;
        return;
    }
    int index;
    struct WCHashEntry * temp;
    for (index = 0 ; index < hash->bound ; ++index) {
        if ((hash->base)[index]) {
            while ((hash->base)[index]->next) {
                temp = (hash->base)[index]->next;
                (hash->base)[index]->next = temp->next;
                if (temp->index) {
                    WCError internalError;
                    wc_index_destroy(temp->index, &internalError);
                    if (internalError != WCNoneError) {
                        exit(internalError);
                    }
                }
                if (temp->word) free(temp->word);
                free(temp);
            }
            free((hash->base)[index]);
        }
    }
    free(hash->base);
    free(hash);
    *error = WCNoneError;
}

void wc_hash_table_insert_word(WCHashTable * hash, WCWord * word, WCError * error) {
    if (hash == NULL || word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCError internalError;
    const char * wordPtr = wc_word_get_word(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    WCWordInfo info = wc_word_get_info(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    int hash_num = RSHash(wordPtr);
    int index = hash_num % hash->bound;
    if ((hash->base)[index] == NULL) {
        (hash->base)[index] = malloc(sizeof(struct WCHashEntry));
        if ((hash->base)[index] == NULL) {
            *error = WCMemoryOverflowError;
            return;
        }
        (hash->base)[index]->word = NULL;
        (hash->base)[index]->index = NULL;
        (hash->base)[index]->next = NULL;
    }
    struct WCHashEntry * temp = (hash->base)[index]->next;
    while (temp) {
        if (temp->word && strcmp(temp->word, wordPtr) == 0) {
            if (temp->index == NULL) exit(WCNullPointerError);
            wc_index_add(temp->index, info, &internalError);
            if (internalError != WCNoneError) {
                exit(internalError);
            }
            *error = WCNoneError;
            return;
        }
        temp = temp->next;
    }
    struct WCHashEntry * newEntry = malloc(sizeof(struct WCHashEntry));
    int length = wc_word_get_length(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    newEntry->word = malloc(sizeof(char) * (length + 1));
    strcpy(newEntry->word, wordPtr);
    newEntry->index = wc_index_create(&internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    wc_index_add(newEntry->index, info, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    newEntry->next = (hash->base)[index]->next;
    (hash->base)[index]->next = newEntry;
    *error = WCNoneError;
}

WCIndex * wc_hash_table_search_word(WCHashTable * hash, WCWord * word, WCError * error) {
    if (hash == NULL || word == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCError internalError;
    const char * wordPtr = wc_word_get_word(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    int hash_num = RSHash(wordPtr);
    int index = hash_num % hash->bound;
    if ((hash->base)[index] == NULL) {
        *error = WCNoneError;
        return NULL;
    }
    struct WCHashEntry * temp = (hash->base)[index]->next;
    while (temp) {
        if (temp->word && strcmp(temp->word, wordPtr) == 0) {
            *error = WCNoneError;
            return temp->index;
        }
        temp = temp->next;
    }
    *error = WCNoneError;
    return NULL;
}
