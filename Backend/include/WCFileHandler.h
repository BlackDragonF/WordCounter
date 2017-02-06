#ifndef WC_FILE_HANDLER_H
#define WC_FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct WCFileHandler;
typedef struct WCFileHandler WCFileHandler;

WCFileHandler * wc_file_handler_create(WCError * error);

void wc_file_open_book(WCFileHandler * handler, const char * path, WCError * error);

void wc_file_close(WCFileHandler * handler, WCError * error);

void wc_file_handler_destroy(WCFileHandler * handler, WCError * error);




#endif
