#include "WCHashTable.h"

/* struct definition */
struct WCHashEntry {
    char * word;
    WCIndex * index;
    struct WCHashEntry * next;
};

struct WCHashTable {
    int bound;
    struct WCHashEntry ** base;
    int count;
    int unique;
};

/* static hash function */
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

/* construct a WCHashTable struct */
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
    hash->count = 0;
    hash->unique = 0;
    *error = WCNoneError;
    return hash;
}

/* destruct given WCHashTable struct */
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

/* insert word into hash table */
void wc_hash_table_insert_word(WCHashTable * hash, WCWord * word, WCError * error) {
    if (hash == NULL || word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCError internalError;
    hash->count++;
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
    hash->unique++;
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

/* search word in hash table */
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

/* delete word in hash table */
void wc_hash_table_delete_word(WCHashTable * hash, WCWord * word, WCError * error) {
    if (hash == NULL || word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCError internalError;
    const char * wordPtr = wc_word_get_word(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    int hash_num = RSHash(wordPtr);
    int index = hash_num % hash->bound;
    if ((hash->base)[index] != NULL) {
        struct WCHashEntry * temp = (hash->base)[index];
        while(temp->next) {
            if (temp->next->word && strcmp(temp->next->word, wordPtr) == 0) {
                struct WCHashEntry * p = temp->next;
                int count = wc_index_get_count(p->index, &internalError);
                if (internalError != WCNoneError) {
                    exit(internalError);
                }
                hash->count -= count;
                temp->next = p->next;
                if (p->index) {
                    wc_index_destroy(p->index, &internalError);
                    if (internalError != WCNoneError) {
                        exit(internalError);
                    }
                }
                if (p->word) free(p->word);
                free(p);
                *error = WCNoneError;
                return;
            }
            temp = temp->next;
        }
    }
    *error = WCNoneError;
}

/* struct definition */
struct WCHashTableIterator {
    WCHashTable * hash;
    int index;
    struct WCHashEntry * entry;
};

/* construct a WCHashTableIterator struct */
WCHashTableIterator * wc_hash_table_iterator_create(WCHashTable * hash, WCError * error) {
    if (hash == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCHashTableIterator * iterator = malloc(sizeof(WCHashTableIterator));
    iterator->hash = hash;
    int index = 0;
    while (index < hash->bound) {
        if ((hash->base)[index] != NULL) {
            struct WCHashEntry * entry = (hash->base)[index]->next;
            if (entry != NULL) {
                iterator->index = index;
                iterator->entry = entry;
                *error = WCNoneError;
                return iterator;
            }
        }
        index++;
    }
    *error = WCIndexRangeError;
    free(iterator);
    return NULL;
}

/* destruct a WCHashTableIterator struct */
void wc_hash_table_iterator_destroy(WCHashTableIterator * iterator, WCError * error) {
    if (iterator == NULL) {
        *error = WCNullPointerError;
        return;
    }
    free(iterator);
    *error = WCNoneError;
}

/* move iterator to next */
void wc_hash_table_iterator_next(WCHashTableIterator * iterator, WCError * error) {
    if (iterator == NULL) {
        *error = WCNullPointerError;
        return;
    }
    iterator->entry = iterator->entry->next;
    if (iterator->entry == NULL) {
        iterator->index++;
        while (iterator->index < iterator->hash->bound) {
            if ((iterator->hash->base)[iterator->index] != NULL) {
                struct WCHashEntry * entry = (iterator->hash->base)[iterator->index]->next;
                if (entry != NULL) {
                    iterator->entry = entry;
                    *error = WCNoneError;
                    return;
                }
            }
            iterator->index++;
        }
    } else {
        *error = WCNoneError;
        return;
    }
    *error = WCIndexRangeError;
}

/* get value index for current iterator */
WCIndex * wc_hash_table_iterator_get_index(WCHashTableIterator * iterator, char ** word, WCError * error) {
    if (iterator == NULL || word == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    *word = iterator->entry->word;
    *error = WCNoneError;
    return iterator->entry->index;
}

/* serializer for WCHashTable struct */
static void wc_struct_hash_table_serialize(WCHashTable * hash, FILE * fp, WCError * error) {
    if (hash == NULL || fp == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCObjectTag tag = WCHashTableObject;
    fwrite(&tag, sizeof(WCObjectTag), 1, fp);
    fwrite(hash, sizeof(WCHashTable), 1, fp);
    *error = WCNoneError;
}

/* deserializer for WCHashTable struct */
static void wc_struct_hash_table_deserialize(WCHashTable * hash, FILE * fp, WCError * error) {
    if (hash == NULL || fp == NULL) {
        *error = WCNullPointerError;
        return;
    }
    fread(hash, sizeof(WCHashTable), 1, fp);
    *error = WCNoneError;
}

/* serializer for WCHashEntry struct */
static void wc_struct_hash_entry_serialize(struct WCHashEntry * entry, int index, FILE * fp, WCError * error) {
    if (entry == NULL || fp == NULL) {
        *error = WCNullPointerError;
        return;
    }
    if (index < 0) {
        *error = WCIndexRangeError;
        return;
    }
    WCObjectTag tag = WCHashEntryObject;
    fwrite(&tag, sizeof(WCObjectTag), 1, fp);
    fwrite(&index, sizeof(int), 1, fp);
    fwrite(entry, sizeof(struct WCHashEntry), 1, fp);
    *error = WCNoneError;
}

/* deserializer for WCHashEntry struct */
static void wc_struct_hash_entry_deserialize(struct WCHashEntry * entry, FILE * fp, WCError * error) {
    if (entry == NULL || fp == NULL) {
        *error = WCNullPointerError;
        return;
    }
    fread(entry, sizeof(struct WCHashEntry), 1, fp);
    *error = WCNoneError;
}

/* serialize the whole hash table into a binary file */
void wc_hash_table_write_to_file(WCHashTable * hash, WCFileHandler * handler, const char * path, WCError * error) {
    if (hash == NULL || handler == NULL || path == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCError internalError;
    wc_file_open_hash(handler, path, Write, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    FILE * fp = wc_file_handler_get_file_pointer(handler, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    wc_struct_hash_table_serialize(hash, fp, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    int index;
    for (index = 0 ; index < hash->bound ; ++index) {
        if ((hash->base)[index] != NULL) {
            // Head entry is NOT INCLUDED
            struct WCHashEntry * temp = (hash->base)[index]->next;
            while (temp) {
                wc_struct_hash_entry_serialize(temp, index, fp, &internalError);
                if (internalError != WCNoneError) {
                    exit(internalError);
                }
                if (temp->word == NULL) {
                    wc_struct_null_serialize(fp, &internalError);
                    if (internalError != WCNoneError) {
                        exit(internalError);
                    }
                } else {
                    wc_struct_string_serialize(temp->word, (int)strlen(temp->word) + 1, fp, &internalError);
                    if (internalError != WCNoneError) {
                        exit(internalError);
                    }
                }
                wc_struct_index_serialize(temp->index, fp, &internalError);
                if (internalError != WCNoneError) {
                    exit(internalError);
                }
                temp = temp->next;
            }
        }
    }
    wc_file_close(handler, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    *error = WCNoneError;
}

/* load the hash table from given binary file */
WCHashTable * wc_hash_table_read_from_file(WCFileHandler * handler, const char * path, WCError * error) {
    if (handler == NULL || path == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCError internalError;
    wc_file_open_hash(handler, path, Read, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    FILE * fp = wc_file_handler_get_file_pointer(handler, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    WCObjectTag tag;
    fread(&tag, sizeof(WCObjectTag), 1, fp);
    if (tag != WCHashTableObject) {
        *error = WCFileInternalError;
        wc_file_close(handler, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        return NULL;
    }
    WCHashTable * hash = malloc(sizeof(WCHashTable));
    if (hash == NULL) {
        *error = WCMemoryOverflowError;
        wc_file_close(handler, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        return NULL;
    }
    wc_struct_hash_table_deserialize(hash, fp, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    hash->base = malloc(sizeof(struct WCHashEntry *) * hash->bound);
    if (hash->base == NULL) {
        *error = WCMemoryOverflowError;
        wc_file_close(handler, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        free(hash);
        return NULL;
    }
    memset(hash->base, 0, sizeof(struct WCHashEntry *) * hash->bound);
    int count;
    int index;
    for (count = 0 ; count < hash->count ;) {
        fread(&tag, sizeof(WCObjectTag), 1, fp);
        if (tag != WCHashEntryObject) {
            exit(WCFileInternalError);
        }
        fread(&index, sizeof(int), 1, fp);
        if ((hash->base)[index] == NULL) {
            (hash->base)[index] = malloc(sizeof(struct WCHashEntry));
            if ((hash->base)[index] == NULL) {
                exit(WCMemoryOverflowError);
            }
            (hash->base)[index]->word = NULL;
            (hash->base)[index]->index = NULL;
            (hash->base)[index]->next = NULL;
        }
        struct WCHashEntry * newEntry = malloc(sizeof(struct WCHashEntry));
        wc_struct_hash_entry_deserialize(newEntry, fp, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        fread(&tag, sizeof(WCObjectTag), 1, fp);
        if (tag == WCNullObject) {
            newEntry->word = NULL;
        } else if (tag == WCStringObject) {
            newEntry->word = wc_struct_string_deserialize(fp, &internalError);
            if (internalError != WCNoneError) {
                exit(internalError);
            }
        } else {
            exit(WCFileInternalError);
        }
        fread(&tag, sizeof(WCObjectTag), 1, fp);
        if (tag != WCIndexObject) {
            exit(WCFileInternalError);
        }
        newEntry->index = wc_struct_index_deserialize(fp, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        newEntry->next = (hash->base)[index]->next;
        (hash->base)[index]->next = newEntry;
        int subcount = wc_index_get_count(newEntry->index, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        count += subcount;
    }
    wc_file_close(handler, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    *error = WCNoneError;
    return hash;
}

/* get count from given WCHashTable struct */
int wc_hash_table_get_count(WCHashTable * hash, WCError * error) {
    if (hash == NULL) {
        *error = WCNullPointerError;
        return 0;
    }
    *error = WCNoneError;
    return hash->count;
}

/* get unique from given WCHashTable struct */
int wc_hash_table_get_unique(WCHashTable * hash, WCError * error) {
    if (hash == NULL) {
        *error = WCNullPointerError;
        return 0;
    }
    *error = WCNoneError;
    return hash->unique;
}
