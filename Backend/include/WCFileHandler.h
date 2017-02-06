#ifndef WC_FILE_HANDLER_H
#define WC_FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WCErrorHandler.h"

typedef enum WCFileType {Book, Trie, Hash, None} WCFileType;
typedef enum WCFileState {Open, Closed} WCFileState;

struct WCFileHandler;
typedef struct WCFileHandler WCFileHandler;

WCFileHandler * wc_file_handler_create(WCError * error);
WCFileType wc_file_handler_get_type(WCFileHandler * handler, WCError * error);
WCFileState wc_file_handler_get_state(WCFileHandler * handler, WCError * error);
FILE * wc_file_handler_get_file_pointer(WCFileHandler * handler, WCError * error);

void wc_file_open_book(WCFileHandler * handler, const char * path, WCError * error);

void wc_file_close(WCFileHandler * handler, WCError * error);

void wc_file_handler_destroy(WCFileHandler * handler, WCError * error);




#endif
