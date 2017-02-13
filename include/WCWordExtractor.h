#ifndef WC_WORD_EXTRACTOR_H
#define WC_WORD_EXTRACTOR_H

/* include necessary libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* include word/error/file modules */
#include "WCWord.h"
#include "WCErrorHandler.h"
#include "WCFileHandler.h"

/* macro definition */
#define WC_CHAR_BUFFER_SIZE (1024)

/* struct declaration */
struct WCWordExtractor;
typedef struct WCWordExtractor WCWordExtractor;

/* function list */

/* construct and destruct */
WCWordExtractor * wc_word_extractor_create_with_file(WCFileHandler * handler, WCError * error);
void wc_word_extractor_destroy(WCWordExtractor * extractor, WCError * error);

/* iteration related */
WCWord * wc_word_extractor_next_word(WCWordExtractor * extractor, WCError * error);

#endif
