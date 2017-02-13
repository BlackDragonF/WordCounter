#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

/* include required modules */
#include "WCErrorHandler.h"
#include "WCFileHandler.h"
#include "WCWord.h"
#include "WCIndex.h"
#include "WCWordExtractor.h"
#include "WCTrieTree.h"
#include "WCHashTable.h"
#include "WCClock.h"

/* header for argument parsing */
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* enum definition */
typedef enum WCSourceFileType {WCSourceFileTypeText, WCSourceFileTypeData} WCSourceFileType;
typedef enum WCStructType {WCStructTypeTrieTree, WCStructTypeHashTable, WCStructTypeBoth} WCStructType;

/* macro definition */
#define WC_DEFAULT_HASH_SIZE (1024)
#define WC_MAX_ARGUMENT_LENGTH (128)
#define WC_MAX_INPUT_LENGTH (128)

#endif
