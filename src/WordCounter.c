#include "WordCounter.h"

/* global wc_clock for timing */
WCClock * wc_clock = NULL;

/* print the help text */
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

/* print the traverse header */
static void wc_print_traverse_header(void) {
    printf(" --------------------------------------- ------------------- ------------------ \n");
    printf("|Word                                   |Row                |Column            |\n");
    printf(" --------------------------------------- ------------------- ------------------ \n");
}

/* print the search result header */
static void wc_print_search_result_header(void) {
    printf(" ------------------- ------------------ \n");
    printf("|Row                |Column            |\n");
    printf(" ------------------- ------------------ \n");
}

/* construct the hash table from data file */
static WCHashTable * wc_construct_hash_table_from_data(WCFileHandler * handler, const char * path) {
    WCError error;
    WCHashTable * hash = wc_hash_table_read_from_file(handler, path, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to construct hash table from given file\n");
        exit(error);
    }
    return hash;
}

/* save the hash table to data file */
static void wc_save_hash_table_to_data(WCHashTable * hash, WCFileHandler * handler, const char * path) {
    WCError error;
    char target[strlen(path) + 4];
    strcpy(target, path);
    strcat(target, ".dat");
    wc_hash_table_write_to_file(hash, handler, target, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to save hash table to target file\n");
        exit(error);
    }
}

/* construct the hash table from text file */
static WCHashTable * wc_construct_hash_table_from_text(WCFileHandler * handler, const char * path) {
    WCError error;

    WCHashTable * hash = NULL;

    printf("INFO: Constructing hash table from text file...\n");
    wc_clock_start(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to start clock\n");
    }
    wc_file_open_book(handler, path, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to open text file: %s\n", path);
        exit(error);
    }
    WCWordExtractor * extractor = wc_word_extractor_create_with_file(handler, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to create WCWordExtractor\n");
        exit(error);
    }
    WCWord * word;
    hash = wc_hash_table_create(WC_DEFAULT_HASH_SIZE, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to create WCHashTable\n");
        exit(error);
    }
    while ((word = wc_word_extractor_next_word(extractor, &error))) {
        if (error != WCNoneError) {
            printf("FETAL: Failed to extract word\n");
            exit(error);
        }
        WCError internalError;
        wc_hash_table_insert_word(hash, word, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: Failed to insert word into WCHashTable\n");
            exit(internalError);
        }
        wc_word_destroy(word, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: Failed to destroy WCWord\n");
            exit(internalError);
        }
    }
    wc_word_extractor_destroy(extractor, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to destroy WCWordExtractor\n");
        exit(error);
    }
    wc_file_close(handler, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to close file\n");
        exit(error);
    }
    wc_clock_stop(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to stop clock\n");
    }
    double interval = wc_clock_get_interval(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get interval from WCClock\n");
    }
    int unique = wc_hash_table_get_unique(hash, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get unique from WCHashTable\n");
    }
    int count = wc_hash_table_get_count(hash, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get count from WCHashTable\n");
        printf("INFO: Constructing process succeed, using %.3lfs in total\n", interval);
    } else {
        printf("INFO: Constructing process succeed, %d words in total, %d unique, using %.3lfs in total\n", count, unique, interval);
    }
    return hash;
}

/* construct the trie tree from text file */
static WCTrieTree * wc_construct_trie_tree_from_text(WCFileHandler * handler, const char * path) {
    WCError error;

    WCTrieTree * trie = NULL;

    printf("INFO: Constructing trie tree from text file...\n");
    wc_clock_start(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to start clock\n");
    }
    wc_file_open_book(handler, path, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to open text file: %s\n", path);
        exit(error);
    }
    WCWordExtractor * extractor = wc_word_extractor_create_with_file(handler, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to create WCWordExtractor\n");
        exit(error);
    }
    WCWord * word;
    trie = wc_trie_tree_create(&error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to create WCTrieTree\n");
        exit(error);
    }
    while ((word = wc_word_extractor_next_word(extractor, &error))) {
        if (error != WCNoneError) {
            printf("FETAL: Failed to extract word\n");
            exit(error);
        }
        WCError internalError;
        wc_trie_tree_insert_word(trie, word, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: Failed to insert word into WCTrieTree\n");
            exit(internalError);
        }
        wc_word_destroy(word, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: Failed to destroy WCWord\n");
            exit(internalError);
        }
    }
    wc_word_extractor_destroy(extractor, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to destroy WCWordExtractor\n");
        exit(error);
    }
    wc_file_close(handler, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to close file\n");
        exit(error);
    }
    wc_clock_stop(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to stop clock\n");
    }
    double interval = wc_clock_get_interval(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get interval from WCClock\n");
    }
    int unique = wc_trie_tree_get_unique(trie, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get unique from WCTrieTree\n");
    }
    int count = wc_trie_tree_get_count(trie, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get count from WCTrieTree\n");
        printf("INFO: Constructing process succeed, using %.3lfs in total\n", interval);
    } else {
        printf("INFO: Constructing process succeed, %d words in total, %d unique, using %.3lfs in total\n", count, unique, interval);
    }
    return trie;
}

/* wrapper for construct look-up table from text file */
static void wc_construct_structure_from_text(WCStructType type, WCHashTable ** phash, WCTrieTree ** ptrie, WCFileHandler * handler, const char * path) {
    switch (type) {
        case WCStructTypeHashTable:
            *phash = wc_construct_hash_table_from_text(handler, path);
            break;
        case WCStructTypeTrieTree:
            *ptrie = wc_construct_trie_tree_from_text(handler, path);
            break;
        case WCStructTypeBoth:
            *phash = wc_construct_hash_table_from_text(handler, path);
            *ptrie = wc_construct_trie_tree_from_text(handler, path);
            break;
    }
}

/* traverse the hash table through iterator */
static void wc_hash_table_traverse_wrapper(WCHashTable * hash) {
    WCError error;
    int count;
    printf("INFO: Starting traverse hash table...\n");
    wc_clock_start(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to start clock\n");
    }
    WCHashTableIterator * iterator = wc_hash_table_iterator_create(hash, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to create iterator for WCHashTable\n");
        exit(error);
    }
    wc_print_traverse_header();
    char * word;
    do {
        WCError internalError;
        WCIndex * index = wc_hash_table_iterator_get_index(iterator, &word, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: Failed to get index through iterator\n");
            exit(internalError);
        }
        WCIndexIterator * indexIterator = wc_index_iterator_create(index, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: Failed to create iterator for WCIndex\n");
            exit(internalError);
        }
        do {
            WCWordInfo info = wc_index_iterator_get_value(indexIterator, &internalError);
            if (internalError != WCNoneError) {
                printf("FETAL: Failed to get info through iterator\n");
                exit(internalError);
            }
            printf("|%-39s|%-19d|%-18d|\n", word, info.row, info.column);
            count++;
        } while (wc_index_iterator_next(indexIterator, &internalError), internalError != WCIndexRangeError);
        wc_index_iterator_destroy(indexIterator, &internalError);
        if (internalError != WCNoneError) {
            printf("FETAL: Failed to destroy WCIndexIterator\n");
            exit(internalError);
        }
    } while (wc_hash_table_iterator_next(iterator, &error), error != WCIndexRangeError);
    wc_hash_table_iterator_destroy(iterator, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to destroy WCHashTableIterator\n");
        exit(error);
    }
    printf(" --------------------------------------- ------------------- ------------------ \n");
    wc_clock_stop(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to stop clock\n");
    }
    int unique = wc_hash_table_get_unique(hash, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get unique from WCHashTable\n");
    }
    double interval = wc_clock_get_interval(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get interval from WCClock\n");
    }
    printf("INFO: Traversal process succeed, %d words in total, %d unique, using %.3lfs in total\n", count, unique, interval);
}

/* traverse the trie tree through array */
static void wc_trie_tree_traverse_wrapper(WCTrieTree * trie) {
    WCError error;
    printf("INFO: Starting traverse trie tree...\n");
    wc_clock_start(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to start clock\n");
    }
    WCTrieTreeTraverseResult * result = wc_trie_tree_traverse(trie, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to create traverse result for trie tree\n");
        exit(error);
    }
    wc_print_traverse_header();
    int index;
    char * word = NULL;
    for (index = 0 ; index < result->count ; ++index) {
        if ((result->words)[index] != NULL) {
            word = (result->words)[index];
        }
        printf("|%-39s|%-19d|%-18d|\n", word, (result->info)[index].row, (result->info)[index].column);
    }
    int count = result->count;
    wc_trie_tree_traverse_result_destroy(result, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to destroy WCTrieTreeTraverseResult\n");
        exit(error);
    }
    printf(" --------------------------------------- ------------------- ------------------ \n");
    wc_clock_stop(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to stop clock\n");
    }
    int unique = wc_trie_tree_get_unique(trie, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get unique from WCTrieTree\n");
    }
    double interval = wc_clock_get_interval(wc_clock, &error);
    if (error != WCNoneError) {
        printf("ERROR: Failed to get interval from WCClock\n");
    }
    printf("INFO: Traversal process succeed, %d words in total, %d unique, using %.3lfs in total\n", count, unique, interval);
}

/* wrapper for search certain word in hash table */
static void wc_hash_table_search_word_wrapper(WCHashTable * hash, WCWord * word) {
    WCError error;
    WCIndexIterator * iterator;
    WCIndex * index = wc_hash_table_search_word(hash, word, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to search word in WCHashTable\n");
        exit(error);
    }
    const char * wordPtr = wc_word_get_word(word, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to get word pointer in WCWord\n");
        exit(error);
    }
    if (index == NULL) {
        printf("INFO: No word %s found\n", wordPtr);
    } else {
        int count = wc_index_get_count(index, &error);
        if (error != WCNoneError) {
            printf("ERROR: Failed to get count from WCIndex\n");
        } else {
            printf("There are %d words matched\n", count);
        }
        printf("The search result for word %s is:\n", wordPtr);
        wc_print_search_result_header();
        iterator = wc_index_iterator_create(index, &error);
        if (error != WCNoneError) {
            exit(error);
        }
        do {
            WCError internalError;
            WCWordInfo info = wc_index_iterator_get_value(iterator, &internalError);
            if (internalError != WCNoneError) {
                printf("FETAL: Failed to get info through iterator\n");
                exit(internalError);
            }
            printf("|%-19d|%-18d|\n", info.row, info.column);
        } while (wc_index_iterator_next(iterator, &error), error != WCIndexRangeError);
        wc_index_iterator_destroy(iterator, &error);
        if (error != WCNoneError) {
            exit(error);
        }
        printf(" ------------------- ------------------ \n");
    }
}

/* wrapper for search certain word in trie tree */
static void wc_trie_tree_search_word_wrapper(WCTrieTree * trie, WCWord * word) {
    WCError error;
    WCIndexIterator * iterator;
    WCIndex * index = wc_trie_tree_search_word(trie, word, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to search word in WCTrieTree\n");
        exit(error);
    }
    const char * wordPtr = wc_word_get_word(word, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to get word pointer in WCWord\n");
        exit(error);
    }
    if (index == NULL) {
        printf("INFO: No word %s found\n", wordPtr);
    } else {
        int count = wc_index_get_count(index, &error);
        if (error != WCNoneError) {
            printf("ERROR: Failed to get count from WCIndex\n");
        } else {
            printf("There are %d words matched\n", count);
        }
        printf("The search result for word %s is:\n", wordPtr);
        wc_print_search_result_header();
        iterator = wc_index_iterator_create(index, &error);
        if (error != WCNoneError) {
            exit(error);
        }
        do {
            WCError internalError;
            WCWordInfo info = wc_index_iterator_get_value(iterator, &internalError);
            if (internalError != WCNoneError) {
                printf("FETAL: Failed to get info through iterator\n");
                exit(internalError);
            }
            printf("|%-19d|%-18d|\n", info.row, info.column);
        } while (wc_index_iterator_next(iterator, &error), error != WCIndexRangeError);
        wc_index_iterator_destroy(iterator, &error);
        if (error != WCNoneError) {
            exit(error);
        }
        printf(" ------------------- ------------------ \n");
    }
}

/* wrapper for search words in look-up tables */
static void wc_search_word_wrapper(WCStructType type, WCHashTable * hash, WCTrieTree * trie) {
    char input[WC_MAX_INPUT_LENGTH];
    double interval;
    WCError error;
    WCWord * word = wc_word_create(WC_DEFAULT_WORD_LENGTH, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to create WCWord\n");
        exit(error);
    }
    while (printf("Please input the word you want to search, '!' to end: "), scanf("%s", input), strcmp(input, "!") != 0) {
        wc_word_set_word(word, input, &error);
        if (error != WCNoneError) {
            printf("FETAL: Failed to set word for WCWord\n");
            exit(error);
        }
        switch (type) {
            case WCStructTypeHashTable:
                printf("INFO: Searching %s in hash table\n", input);
                wc_clock_start(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to start clock\n");
                }
                wc_hash_table_search_word_wrapper(hash, word);
                wc_clock_stop(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to stop clock\n");
                }
                interval = wc_clock_get_interval(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to get interval from WCClock\n");
                }
                printf("INFO: Searching process succeed, using %.3lfs in total\n", interval);
                break;
            case WCStructTypeTrieTree:
                printf("INFO: Searching %s in trie tree\n", input);
                wc_clock_start(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to start clock\n");
                }
                wc_trie_tree_search_word_wrapper(trie, word);
                wc_clock_stop(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to stop clock\n");
                }
                interval = wc_clock_get_interval(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to get interval from WCClock\n");
                }
                printf("INFO: Searching process succeed, using %.3lfs in total\n", interval);
                break;
            case WCStructTypeBoth:
                printf("INFO: Searching %s in hash table\n", input);
                wc_clock_start(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to start clock\n");
                }
                wc_hash_table_search_word_wrapper(hash, word);
                wc_clock_stop(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to stop clock\n");
                }
                interval = wc_clock_get_interval(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to get interval from WCClock\n");
                }
                printf("INFO: Searching process succeed, using %.3lfs in total\n", interval);
                printf("INFO: Searching %s in trie tree\n", input);
                wc_clock_start(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to start clock\n");
                }
                wc_trie_tree_search_word_wrapper(trie, word);
                wc_clock_stop(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to stop clock\n");
                }
                interval = wc_clock_get_interval(wc_clock, &error);
                if (error != WCNoneError) {
                    printf("ERROR: Failed to get interval from WCClock\n");
                }
                printf("INFO: Searching process succeed, using %.3lfs in total\n", interval);
                break;
        }
        wc_word_clean(word, &error);
        if (error != WCNoneError) {
            printf("FETAL: Failed to clean WCWord\n");
            exit(error);
        }
    }
}

/* main entry */
int main(int argc, char *argv[]) {
    /* variable declarations and definitions */
    int opt = 0;
    char * path = argv[argc - 1];
    char arg_s[WC_MAX_ARGUMENT_LENGTH];

    WCError error;

    int h = 0, d = 0, s = 0, k = 0, f = 0, t = 0;

    WCSourceFileType file_type = WCSourceFileTypeText;
    WCStructType struct_type = WCStructTypeBoth;

    WCHashTable * hash = NULL;
    WCTrieTree * trie = NULL;

    printf("WordCounter - MIT License(c) BlackDragon - obsidiandragon2016@gmail.com\n");

    /* initialize the handler */
    WCFileHandler * handler = wc_file_handler_create(&error);

    if (error != WCNoneError) {
        printf("FETAL: failed to create file handler\n");
        return (error);
    }

    /* argument parse */
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

    /* -h */
    if (h == 1) {
        wc_show_help_text();
        return 0;
    }

    /* initialize the clock */
    wc_clock = wc_clock_create(&error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to create WCClock\n");
        exit(error);
    }

    /* -d */
    if (d == 1) {
        file_type = WCSourceFileTypeData;
        struct_type = WCStructTypeHashTable;
        printf("INFO: Constructing look-up table from data file...\n");
        wc_clock_start(wc_clock, &error);
        if (error != WCNoneError) {
            printf("ERROR: Failed to start clock\n");
        }
        hash = wc_construct_hash_table_from_data(handler, path);
        wc_clock_stop(wc_clock, &error);
        if (error != WCNoneError) {
            printf("ERROR: Failed to stop clock\n");
        }
        double interval = wc_clock_get_interval(wc_clock, &error);
        if (error != WCNoneError) {
            printf("ERROR: Failed to get interval from WCClock\n");
        }
        int unique = wc_hash_table_get_unique(hash, &error);
        if (error != WCNoneError) {
            printf("ERROR: Failed to get unique from WCHashTable\n");
        }
        int count = wc_hash_table_get_count(hash, &error);
        if (error != WCNoneError) {
            printf("ERROR: Failed to get count from WCHashTable\n");
            printf("INFO: Constructing process succeed, using %.3lfs in total\n", interval);
        } else {
            printf("INFO: Constructing process succeed, %d words in total, %d unique, using %.3lfs in total\n", count, unique, interval);
        }
    }
    /* -s */
    if (s == 1 && d == 0) {
        if(strcmp("hash", arg_s) == 0) {
            struct_type = WCStructTypeHashTable;
        } else if (strcmp("trie", arg_s) == 0) {
            struct_type = WCStructTypeTrieTree;
        } else if (strcmp("both", arg_s) == 0) {
            struct_type = WCStructTypeBoth;
        } else {
            printf("WARNING: Failed to parse argument -s, use default both\n");
            struct_type = WCStructTypeBoth;
        }
        wc_construct_structure_from_text(struct_type, &hash, &trie, handler, path);
        /* -k */
        if (k == 1 && (struct_type == WCStructTypeBoth || struct_type == WCStructTypeHashTable)) {
            printf("INFO: Saving hash table to data file...\n");
            wc_clock_start(wc_clock, &error);
            if (error != WCNoneError) {
                printf("ERROR: Failed to start clock\n");
            }
            wc_save_hash_table_to_data(hash, handler, path);
            wc_clock_stop(wc_clock, &error);
            if (error != WCNoneError) {
                printf("ERROR: Failed to stop clock\n");
            }
            double interval = wc_clock_get_interval(wc_clock, &error);
            if (error != WCNoneError) {
                printf("ERROR: Failed to get interval from WCClock\n");
            }
            printf("INFO: Saving process succeed, using %.3lfs in total\n", interval);
        }
    }
    /* -t */
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
    /* -f */
    if (f == 1) {
        wc_search_word_wrapper(struct_type, hash, trie);
    }
    wc_clock_destroy(wc_clock, &error);
    if (error != WCNoneError) {
        printf("FETAL: Failed to destroy WCClock\n");
        exit(error);
    }
    return 0;
}
