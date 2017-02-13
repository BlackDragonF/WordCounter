#include "WCFileHandler.h"

/* struct definition */
struct WCFileHandler {
    FILE * fp;
    WCFileType type;
    WCFileState state;
    WCFileReadwrite readwrite;
};

/* construct a WCFileHandler struct */
WCFileHandler * wc_file_handler_create(WCError * error) {
    WCFileHandler * handler = malloc(sizeof(WCFileHandler));
    if (handler == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    handler->fp = NULL;
    handler->type = None;
    handler->state = Closed;
    handler->readwrite = Read;
    *error = WCNoneError;
    return handler;
}

/* destruct given WCFileHandler struct */
WCFileType wc_file_handler_get_type(WCFileHandler * handler, WCError * error) {
    if (handler == NULL) {
        *error = WCNullPointerError;
        return None;
    }
    *error = WCNoneError;
    return handler->type;
}

/* get current state Open/Closed for given WCFileHandler struct */
WCFileState wc_file_handler_get_state(WCFileHandler * handler, WCError *error){
    if (handler == NULL) {
        *error = WCNullPointerError;
        return Closed;
    }
    *error = WCNoneError;
    return handler->state;
}

/* get current readwrite Read/Write for given WCFileHandler struct */
WCFileReadwrite wc_file_handler_get_readwrite(WCFileHandler * handler, WCError * error) {
    if (handler == NULL) {
        *error = WCNullPointerError;
        return Read;
    }
    *error = WCNoneError;
    return handler->readwrite;
}

/* get current FILE * for given WCFileHandler struct */
FILE * wc_file_handler_get_file_pointer(WCFileHandler * handler, WCError * error) {
    if (handler == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    *error = WCNoneError;
    return handler->fp;
}

/* open the book in text mode */
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

/* open the hash table file in binary mode */
void wc_file_open_hash(WCFileHandler * handler, const char * path, WCFileReadwrite readwrite, WCError * error) {
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
    if (readwrite == Read) {
        handler->fp = fopen(path, "rb");
    } else {
        handler->fp = fopen(path, "wb");
    }
    if (handler->fp == NULL) {
        *error = WCFileOpenError;
        return;
    }
    handler->type = Hash;
    handler->state = Open;
    handler->readwrite = readwrite;
    *error = WCNoneError;
}

/* open the trie tree file in binary mode */
void wc_file_open_trie(WCFileHandler * handler, const char * path, WCFileReadwrite readwrite, WCError * error) {
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
    if (readwrite == Read) {
        handler->fp = fopen(path, "rb");
    } else {
        handler->fp = fopen(path, "wb");
    }
    if (handler->fp == NULL) {
        *error = WCFileOpenError;
        return;
    }
    handler->type = Trie;
    handler->state = Open;
    handler->readwrite = readwrite;
    *error = WCNoneError;
}

/* close the file kept by current WCFileHandler */
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

/* destruct given WCFileHandler struct */
void wc_file_handler_destroy(WCFileHandler * handler, WCError * error) {
    WCError internalError = WCNoneError;
    if (handler->state == Open) {
        wc_file_close(handler, &internalError);
    }
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    free(handler);
    *error = WCNoneError;
}

/* generate tag for NULL */
void wc_struct_null_serialize(FILE * fp, WCError * error) {
    if (fp == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCObjectTag tag = WCNullObject;
    fwrite(&tag, sizeof(WCObjectTag), 1, fp);
    *error = WCNoneError;
}

/* string serializer */
void wc_struct_string_serialize(const char * string, int length, FILE * fp, WCError * error) {
    if (string == NULL || fp == NULL) {
        *error = WCNullPointerError;
        return;
    }
    if (length <= 0) {
        *error = WCIndexRangeError;
        return;
    }
    WCObjectTag tag = WCStringObject;
    fwrite(&tag, sizeof(WCObjectTag), 1, fp);
    fwrite(&length, sizeof(int), 1, fp);
    fwrite(string, sizeof(char) * length, 1, fp);
    *error = WCNoneError;
}

/* string deserializer */
char * wc_struct_string_deserialize(FILE * fp, WCError * error) {
    if (fp == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    int length;
    fread(&length, sizeof(int), 1, fp);
    char * string = malloc(sizeof(char) * length);
    fread(string, sizeof(char) * length, 1, fp);
    *error = WCNoneError;
    return string;
}
