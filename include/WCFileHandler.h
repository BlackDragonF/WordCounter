#ifndef WC_FILE_HANDLER_H
#define WC_FILE_HANDLER_H

/* include necessary libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* include error module */
#include "WCErrorHandler.h"

/* enum definition */
typedef enum WCFileType {Book, Trie, Hash, None} WCFileType;
typedef enum WCFileState {Open, Closed} WCFileState;
typedef enum WCFileReadwrite {Read, Write} WCFileReadwrite;

typedef enum WCObjectTag {WCHashTableObject = 0, WCHashEntryObject, WCStringObject, WCIndexObject, WCIndexNodeObject, WCNullObject} WCObjectTag;

/* struct declaration */
struct WCFileHandler;
typedef struct WCFileHandler WCFileHandler;

/* function list */

/* construct and destruct */
WCFileHandler * wc_file_handler_create(WCError * error);
void wc_file_handler_destroy(WCFileHandler * handler, WCError * error);

/* get operations */
WCFileType wc_file_handler_get_type(WCFileHandler * handler, WCError * error);
WCFileState wc_file_handler_get_state(WCFileHandler * handler, WCError * error);
WCFileReadwrite wc_file_handler_get_readwrite(WCFileHandler * handler, WCError * error);
FILE * wc_file_handler_get_file_pointer(WCFileHandler * handler, WCError * error);

/* file open and close */
void wc_file_open_book(WCFileHandler * handler, const char * path, WCError * error);
void wc_file_open_hash(WCFileHandler * handler, const char * path, WCFileReadwrite readwrite, WCError * error);
void wc_file_open_trie(WCFileHandler * handler, const char * path, WCFileReadwrite readwrite, WCError * error);
//reverved

void wc_file_close(WCFileHandler * handler, WCError * error);

/* general serialization and deserialization */
void wc_struct_null_serialize(FILE * fp, WCError * error);
void wc_struct_string_serialize(const char * string, int length, FILE * fp, WCError * error);
char * wc_struct_string_deserialize(FILE * fp, WCError * error);

#endif
