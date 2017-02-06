#include "WCWordExtractor.h"

struct WCWordExtractor {
    char buffer[WC_CHAR_BUFFER_SIZE];
    int row;
    int column;
    FILE * fp;
};

WCWordExtractor * wc_word_extractor_create_with_file(WCFileHandler * handler, WCError * error) {
    if (handler == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    if (handler->state != Open || handler->type != Book) {
        *error = WCFileInternalError;
        return NULL;
    }
    WCWordExtractor * extractor = malloc(sizeof(WCWordExtractor));
    if (extractor == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    extractor->

}
