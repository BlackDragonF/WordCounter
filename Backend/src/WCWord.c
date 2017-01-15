#include "WCWord.h"
#include "WCErrorHandler.h"

struct WCWord {
    int length;
    int capacity;
    char * word;
};

static void wc_word_expand_capacity(WCWord * word, int increment, WCError * error) {

}

WCWord * wc_word_create(int capacity, WCError * error) {
    WCWord * word = malloc(sizeof(struct WCWord));
    if (word == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    word->length = 0;
    word->capacity = capacity;
    word->word = malloc(sizeof(char) * capacity);
    if (word->word == NULL) {
        *error = WCMemoryOverflowError;
        free(word);
        return NULL;
    }
    memset(word->word, 0, capacity);
    *error = WCNoneError;
    return word;
}

void wc_word_clean(WCWord * word, WCError * error) {
    memset(word->word, 0, word->capacity);
    word->length = 0;
    *error = WCNoneError;
}

void wc_word_destroy(WCWord * word, WCError * error) {
    free(word->word);
    free(word);
    *error = WCNoneError;
}

void wc_character_expand(WCWord * word, char character, WCError * error) {
    if (word->length >= word->capacity - 1) {
        
    }
    (word->word)[word->length] = character;
    (word->length)++;
    *error = WCNoneError;
}

const char * wc_word_get_word(WCWord * word, WCError * error) {
    *error = WCNoneError;
    return word->word;
}
