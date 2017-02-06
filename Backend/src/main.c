#include "WCErrorHandler.h"
#include "WCFileHandler.h"
#include "WCWord.h"
#include "WCWordExtractor.h"
#include "WCTrieTree.h"

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
    WCTrieTree * trie = wc_trie_tree_create(&error);
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
        wc_word_destroy(temp, &secondaryError);
        if (secondaryError != WCNoneError) {
            exit(secondaryError);
        }
    }
    wc_file_close(handler, &error);
    if (error != WCNoneError) {
        exit(error);
    }
    return 0;
}
