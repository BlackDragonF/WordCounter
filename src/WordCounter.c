#include "WordCounter.h"

static void wc_show_help_text(void) {
    printf("-h				Print this help text and exit\n");
    printf("-d				Use data file(hash table) instead of text file to generate look-up table\n");
    printf("-s				Data structure used to generate look-up table - trie, hash, both - default is both\n");
    printf("				If -d is specified, -s will be ignored\n");
    printf("-k				Save the look-up table(hash table) to file\n");
    printf("				If -s isn't specified with hash or both, -k will be ignored\n");
    printf("-f				Enable interactive find mode\n");
    printf("-t				Print all the words and their positions in text\n");
}

static WCHashTable * wc_construct_hash_table_from_data(WCFileHandler * handler, const char * path) {
    WCError error;
    WCHashTable * hash = wc_hash_table_read_from_file(handler, path, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to construct hash table from given file\n");
        exit(error);
    }
    return hash;
}

static void wc_save_hash_table_to_data(WCHashTable * hash, WCFileHandler * handler, const char * path) {
    WCError error;
    char target[strlen(path) + 4];
    strcpy(target, path);
    strcat(target, ".dat");
    wc_hash_table_write_to_file(hash, handler, target, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to save hash table to target file\n");
        exit(error);
    }
}

static void wc_construct_structure_from_text(WCStructType type, WCHashTable ** phash, WCTrieTree ** ptrie, WCFileHandler * handler, const char * path) {
    WCError error;
    
    WCHashTable * hash = NULL;
    WCTrieTree * trie = NULL;
    
    wc_file_open_book(handler, path, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to open text file: %s\n", path);
        exit(error);
    }
    WCWordExtractor * extractor = wc_word_extractor_create_with_file(handler, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to create WCWordExtractor\n");
        exit(error);
    }
    WCWord * word;
    switch (type) {
        case WCStructTypeHashTable:
            hash = wc_hash_table_create(WC_DEFAULT_HASH_SIZE, &error);
            if (error != WCNoneError) {
                printf("FETAL: failed to create WCHashTable\n");
                exit(error);
            }
            break;
        case WCStructTypeTrieTree:
            trie = wc_trie_tree_create(&error);
            if (error != WCNoneError) {
                printf("FETAL: failed to create WCTrieTree\n");
                exit(error);
            }
            break;
        case WCStructTypeBoth:
            hash = wc_hash_table_create(WC_DEFAULT_HASH_SIZE, &error);
            if (error != WCNoneError) {
                printf("FETAL: failed to create WCHashTable\n");
                exit(error);
            }
            trie = wc_trie_tree_create(&error);
            if (error != WCNoneError) {
                printf("FETAL: failed to create WCTrieTree\n");
                exit(error);
            }
            break;
    }
    while ((word = wc_word_extractor_next_word(extractor, &error))) {
        if (error != WCNoneError) {
            printf("FETAL: failed to extract word\n");
            exit(error);
        }
        WCError internalError;
        switch (type) {
            case WCStructTypeHashTable:
                wc_hash_table_insert_word(hash, word, &internalError);
                if (internalError != WCNoneError) {
                    printf("FETAL: failed to insert word into WCHashTable\n");
                    exit(internalError);
                }
                break;
            case WCStructTypeTrieTree:
                wc_trie_tree_insert_word(trie, word, &internalError);
                if (internalError != WCNoneError) {
                    printf("FETAL: failed to insert word into WCTrieTree\n");
                    exit(internalError);
                }
                break;
            case WCStructTypeBoth:
                wc_hash_table_insert_word(hash, word, &internalError);
                if (internalError != WCNoneError) {
                    printf("FETAL: failed to insert word into WCHashTable\n");
                    exit(internalError);
                }
                wc_trie_tree_insert_word(trie, word, &internalError);
                if (internalError != WCNoneError) {
                    printf("FETAL: failed to insert word into WCTrieTree\n");
                    exit(internalError);
                }
                break;
        }
        wc_word_destroy(word, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: failed to destroy WCWord\n");
            exit(internalError);
        }
    }
    wc_word_extractor_destroy(extractor, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to destroy WCWordExtractor\n");
        exit(error);
    }
    wc_file_close(handler, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to close file\n");
        exit(error);
    }
    switch (type) {
        case WCStructTypeHashTable:
            *phash = hash;
            break;
        case WCStructTypeTrieTree:
            *ptrie = trie;
            break;
        case WCStructTypeBoth:
            *phash = hash;
            *ptrie = trie;
            break;
    }
}

static void wc_hash_table_traverse_wrapper(WCHashTable * hash) {
    WCError error;
    WCHashTableIterator * iterator = wc_hash_table_iterator_create(hash, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to create iterator for WCHashTable\n");
        exit(error);
    }
    char * word;
    do {
        WCError internalError;
        WCIndex * index = wc_hash_table_iterator_get_index(iterator, &word, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: failed to get index through iterator\n");
            exit(internalError);
        }
#warning TODO: Print word
        WCIndexIterator * indexIterator = wc_index_iterator_create(index, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: failed to create iterator for WCIndex\n");
            exit(internalError);
        }
        do {
            WCWordInfo info = wc_index_iterator_get_value(indexIterator, &internalError);
            if (internalError != WCNoneError) {
                printf("FETAL: failed to get info through iterator\n");
                exit(internalError);
            }
#warning TODO: Print info
        } while (wc_index_iterator_next(indexIterator, &internalError), internalError != WCIndexRangeError);
        wc_index_iterator_destroy(indexIterator, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: failed to destroy WCIndexIterator\n");
            exit(internalError);
        }
    } while (wc_hash_table_iterator_next(iterator, &error), error != WCIndexRangeError);
    wc_hash_table_iterator_destroy(iterator, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to destroy WCHashTableIterator\n");
        exit(error);
    }
}

static void wc_trie_tree_traverse_wrapper(WCTrieTree * trie) {
    WCError error;
    WCTrieTreeTraverseResult * result = wc_trie_tree_traverse(trie, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to create traverse result for trie tree\n");
        exit(error);
    }
    int index;
    for (index = 0 ; index < result->count ; ++index) {
#warning TODO: Print word and info
    }
    wc_trie_tree_traverse_result_destroy(result, &error);
    if (error != WCNoneError) {
        printf("FETAL: failed to destroy WCTrieTreeTraverseResult\n");
        exit(error);
    }
}

#warning TODO: Finish search word wrapper

int main(int argc, char *argv[]) {
    int opt = 0;
    char * path = argv[argc - 1];
    char arg_s[WC_MAX_ARGUMENT_LENGTH];
    
    WCError error;
    
    int h = 0, d = 0, s = 0, k = 0, f = 0, t = 0;
    
    WCSourceFileType file_type = WCSourceFileTypeText;
    WCStructType struct_type = WCStructTypeBoth;
    
    WCHashTable * hash = NULL;
    WCTrieTree * trie = NULL;
    
    WCFileHandler * handler = wc_file_handler_create(&error);
    
    if (error != WCNoneError) {
        printf("FETAL: failed to create file handler\n");
        return (error);
    }
    
    while ((opt = getopt(argc, argv, "hds:kft")) != -1) {
        switch (opt) {
            case 'h':
                h = 1;
                break;
            case 'd':
                d = 1;
                break;
            case 's':
                s = 1;
                strcpy(arg_s, optarg);
                break;
            case 'k':
                k = 1;
                break;
            case 'f':
                f = 1;
                break;
            case 't':
                t = 1;
                break;
        }
    }
    if (h == 1) {
        wc_show_help_text();
        return 0;
    }
    if (d == 1) {
        file_type = WCSourceFileTypeData;
        struct_type = WCStructTypeHashTable;
        hash = wc_construct_hash_table_from_data(handler, path);
    }
    if (s == 1 && d == 0) {
        if(strcmp("hash", optarg) == 0) {
            struct_type = WCStructTypeHashTable;
        } else if (strcmp("trie", optarg) == 0) {
            struct_type = WCStructTypeTrieTree;
        } else if (strcmp("both", optarg) == 0) {
            struct_type = WCStructTypeBoth;
        } else {
            printf("WARNING: failed to parse argument -s, use default both");
            struct_type = WCStructTypeBoth;
        }
        wc_construct_structure_from_text(struct_type, &hash, &trie, handler, path);
        if (k == 1 && (struct_type == WCStructTypeBoth || WCStructTypeHashTable)) {
            wc_save_hash_table_to_data(hash, handler, path);
        }
    }
    if (t == 1) {
        switch (struct_type) {
            case WCStructTypeHashTable:
                wc_hash_table_traverse_wrapper(hash);
                break;
            case WCStructTypeTrieTree:
                wc_trie_tree_traverse_wrapper(trie);
                break;
            case WCStructTypeBoth:
                wc_hash_table_traverse_wrapper(hash);
                wc_trie_tree_traverse_wrapper(trie);
                break;
        }
    }
    if (f == 1) {
        
    }
    return 0;
}
