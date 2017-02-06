#include "WCWordExtractor.h"

struct WCWordExtractor {
    char buffer[WC_CHAR_BUFFER_SIZE];
    int row;
    int column;
    FILE * fp;
    char * current;
};

static int wc_word_extractor_fill_buffer(WCWordExtractor * extractor) {
    size_t count;
    if (!feof(extractor->fp)) {
        count = fread(extractor->buffer, sizeof(char), WC_CHAR_BUFFER_SIZE, extractor->fp);
        if (count < WC_CHAR_BUFFER_SIZE) {
            (extractor->buffer)[count] = EOF;
        }
        extractor->current = extractor->buffer;
        return 1;
    }
    return 0;
}

typedef enum ExtractState {Prepare, Character, Other} ExtractState;

WCWordExtractor * wc_word_extractor_create_with_file(WCFileHandler * handler, WCError * error) {
    if (handler == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCError internalError;
    WCFileType type = wc_file_handler_get_type(handler, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    WCFileState state = wc_file_handler_get_state(handler, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    if (state != Open || type != Book) {
        *error = WCFileInternalError;
        return NULL;
    }
    WCWordExtractor * extractor = malloc(sizeof(WCWordExtractor));
    if (extractor == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    extractor->fp = wc_file_handler_get_file_pointer(handler, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    extractor->row = 1;
    extractor->column = 0;
    wc_word_extractor_fill_buffer(extractor);
    extractor->current = extractor->buffer;
    return extractor;
}

void wc_word_extractor_destroy(WCWordExtractor * extractor, WCError * error) {
    if (extractor == NULL) {
        *error = WCNullPointerError;
        return;
    }
    *error = WCNoneError;
    free(extractor);
}

WCWord * wc_word_extractor_next_word(WCWordExtractor * extractor, WCError * error) {
    if (extractor == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCError internalError;
    WCWord * word = NULL;
    ExtractState state = Prepare;
    while (*(extractor->current) != EOF && state != Other) {
        if (state == Prepare) {
            if (isalpha(*(extractor->current))) {
                state = Character;
                word = wc_word_create(WC_DEFAULT_WORD_LENGTH, &internalError);
                if (internalError != WCNoneError) {
                    exit(internalError);
                }
            }
        }
        if (state == Character) {
            if (!isalpha(*(extractor->current))) {
                state = Other;
            } else {
                wc_character_expand(word, *(extractor->current), &internalError);
                if (internalError != WCNoneError) {
                    exit(internalError);
                }
            }
        }
        extractor->current++;
        if (*(extractor->current) == '\n') {
            extractor->row++;
            extractor->column = 0;
        }
        if (extractor->current - extractor->buffer >= WC_CHAR_BUFFER_SIZE) {
            if (wc_word_extractor_fill_buffer(extractor) == 0) {
                return NULL;
            }
        }
    }
    extractor->column++;
    if (word != NULL) {
        WCWordInfo info;
        info.row = extractor->row;
        info.column = extractor->column;
        wc_word_set_info(word, info, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
    }
    return word;
}
