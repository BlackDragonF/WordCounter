#include "WCErrorHandler.h"
#include "WCFileHandler.h"
#include "WCWord.h"
#include "WCWordExtractor.h"
#include "WCTrieTree.h"
#include "WCHashTable.h"
#include "WCIndex.h"

int main(void) {
    WCError error;
    WCFileHandler * handler = wc_file_handler_create(&error);
    if (error != WCNoneError) {
        exit(error);
    }
    wc_file_open_book(handler, "test", &error);
    if (error != WCNoneError) {
        exit(error);
    }
    WCWordExtractor * extractor = wc_word_extractor_create_with_file(handler, &error);
    if (error != WCNoneError) {
        exit(error);
    }
    WCWord * temp;
//    WCTrieTree * trie = wc_trie_tree_create(&error);
    WCHashTable * hash = wc_hash_table_create(1024, &error);
    if (error != WCNoneError) {
        exit(error);
    }
    while ((temp = wc_word_extractor_next_word(extractor, &error))) {
        if (error != WCNoneError) {
            exit(error);
        }
        WCError secondaryError;
        WCWordInfo info = wc_word_get_info(temp, &secondaryError);
        if (secondaryError != WCNoneError) {
            exit(secondaryError);
        }
        printf("%s  %10d    %10d\n", wc_word_get_word(temp, &secondaryError), info.row, info.column);
        if (secondaryError != WCNoneError) {
            exit(secondaryError);
        }
//        wc_trie_tree_insert_word(trie, temp, &secondaryError);
        wc_hash_table_insert_word(hash, temp, &secondaryError);
        if (secondaryError != WCNoneError) {
            exit(secondaryError);
        }
        wc_word_destroy(temp, &secondaryError);
        if (secondaryError != WCNoneError) {
            exit(secondaryError);
        }
    }
    wc_word_extractor_destroy(extractor, &error);
    if (error != WCNoneError) {
        exit(error);
    }
    wc_file_close(handler, &error);
    if (error != WCNoneError) {
        exit(error);
    }
    wc_file_handler_destroy(handler, &error);
    if (error != WCNoneError) {
        exit(error);
    }
    WCWord * word = wc_word_create(WC_DEFAULT_WORD_LENGTH, &error);
    if (error != WCNoneError) {
        exit(error);
    }
    wc_word_set_word(word, "ability", &error);
    if (error != WCNoneError) {
        exit(error);
    }
//    WCIndex * index = wc_trie_tree_search_word(trie, word, &error);
    WCIndex * index = wc_hash_table_search_word(hash, word, &error);
    if (error != WCNoneError) {
        exit(error);
    }
#warning TODO: Fix bug if index is NULL
    WCIndexIterator * iterator = wc_index_iterator_create(index, &error);
    if (error != WCNoneError) {
        exit(error);
    }
    do {
        WCError secondaryError;
        WCWordInfo info = wc_index_iterator_get_value(iterator, &secondaryError);
        if (secondaryError != WCNoneError) {
            exit(secondaryError);
        }
        printf("%10d%10d\n", info.row, info.column);
    } while (wc_index_iterator_next(iterator, &error), error == WCNoneError);
//    wc_trie_tree_destroy(trie, &error);
    wc_hash_table_destroy(hash, &error);
    if (error != WCNoneError) {
        exit(error);
    }
    return 0;
}
