#ifndef WC_WORD_H
#define WC_WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WC_DEFAULT_WORD_LENGTH (20)
#define WC_WORD_LENGTH_INCREMENT (10)

struct WCWord;
typedef struct WCWord WCWord;

WCWord * wc_word_create(int capacity, WCError * error);
void wc_word_clean(WCWord * word, WCError * error);
void wc_word_destroy(WCWord * word, WCError * error);
void wc_character_expand(WCWord * word, char character, WCError * error);
const char * wc_word_get_word(WCWord * word, WCError * error);
void wc_word_set_word(WCWord * wc_word, char * word, WCError * error);
#endif
