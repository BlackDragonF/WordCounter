#include "WCFileHandler.h"

struct WCFileHandler {
    FILE * fp;
    WCFileType type;
    WCFileState state;
};

WCFileHandler * wc_file_handler_create(WCError * error) {
    WCFileHandler * handler = malloc(sizeof(WCFileHandler));
    if (handler == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    handler->fp = NULL;
    handler->type = None;
    handler->state = Closed;
    *error = WCNoneError;
    return handler;
}

WCFileType wc_file_handler_get_type(WCFileHandler * handler, WCError * error) {
    if (handler == NULL) {
        *error = WCNullPointerError;
        return None;
    }
    *error = WCNoneError;
    return handler->type;
}

WCFileState wc_file_handler_get_state(WCFileHandler * handler, WCError *error){
    if (handler == NULL) {
        *error = WCNullPointerError;
        return Closed;
    }
    *error = WCNoneError;
    return handler->state;
}

FILE * wc_file_handler_get_file_pointer(WCFileHandler * handler, WCError * error) {
    if (handler == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    *error = WCNoneError;
    return handler->fp;
}

void wc_file_open_book(WCFileHandler * handler, const char * path, WCError * error) {
    if (handler == NULL || path == NULL) {
        *error = WCNullPointerError;
        return;
    }
    if (handler->type != None) {
        *error = WCFileOpenError;
        return;
    }
    if (handler->state == Open) {
        *error = WCFileOpenError;
        return;
    }
    handler->fp = fopen(path, "r");
    if (handler->fp == NULL) {
        *error = WCFileOpenError;
        return;
    }
    handler->type = Book;
    handler->state = Open;
    *error = WCNoneError;
}

void wc_file_close(WCFileHandler * handler, WCError * error) {
    if (handler->state == Closed) {
        *error = WCFileCloseError;
        return;
    }
    fclose(handler->fp);
    handler->type = None;
    handler->state = Closed;
    *error = WCNoneError;
}

void wc_file_handler_destroy(WCFileHandler * handler, WCError * error) {
    WCError internalError;
    if (handler->state == Open) {
        wc_file_close(handler, &internalError);
    }
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    free(handler);
    *error = WCNoneError;
}
