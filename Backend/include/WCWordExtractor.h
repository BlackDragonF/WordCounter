#ifndef WC_WORD_EXTRACTOR_H
#define WC_WORD_EXTRACTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "WCErrorHandler.h"
#include "WCFileHandler.h"
#include "WCWord.h"

#define WC_CHAR_BUFFER_SIZE (1024)

struct WCWordExtractor;
typedef struct WCWordExtractor WCWordExtractor;

WCWordExtractor * wc_word_extractor_create_with_file(WCFileHandler * handler, WCError * error);
void wc_word_extractor_destroy(WCWordExtractor * extractor, WCError * error);
WCWord * wc_word_extractor_next_word(WCWordExtractor * extractor, WCError * error);
#endif
