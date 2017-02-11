#include "WCWord.h"

struct WCWord {
    int length;
    int capacity;
    char * word;
    WCWordInfo info;
};

static void wc_word_expand_capacity(WCWord * word, int increment, WCError * error) {
    if (word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    if (increment <= 0) {
        *error = WCIndexRangeError;
        return;
    }
    char * temp = realloc(word->word, sizeof(char) * (word->capacity + increment));
    if (temp == NULL) {
        *error = WCMemoryOverflowError;
        return;
    }
    word->word = temp;
    memset(word->word + word->capacity, 0, increment);
    word->capacity += increment;
    *error = WCNoneError;
}

WCWord * wc_word_create(int capacity, WCError * error) {
    WCWord * word = malloc(sizeof(struct WCWord));
    if (word == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    word->length = 0;
    if (capacity <= 0) {
        *error = WCIndexRangeError;
        free(word);
        return NULL;
    }
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
    if (word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    memset(word->word, 0, word->capacity);
    word->length = 0;
    *error = WCNoneError;
}

void wc_word_destroy(WCWord * word, WCError * error) {
    if (word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    free(word->word);
    free(word);
    *error = WCNoneError;
}

void wc_character_expand(WCWord * word, char character, WCError * error) {
    if (word->length >= word->capacity - 1) {
        WCError newError;
        wc_word_expand_capacity(word, WC_WORD_LENGTH_INCREMENT, &newError);
        if (newError) {
            printf("Fetal: Failed to alloc memory for WCWord.\n");
            exit(newError);
        }
    }
    (word->word)[word->length] = character;
    (word->length)++;
    *error = WCNoneError;
}

void wc_character_shrink(WCWord * word, WCError * error) {
    if (word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    (word->word)[word->length] = '\0';
    (word->length)--;
    *error = WCNoneError;
}

const char * wc_word_get_word(WCWord * word, WCError * error) {
    if (word == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    *error = WCNoneError;
    return word->word;
}

void wc_word_set_word(WCWord * wc_word, char * word, WCError * error) {
    if (word == NULL || wc_word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCError newError;
    if (strlen(word) + 1 > wc_word->length) {
        wc_word_expand_capacity(wc_word, (int)(strlen(word) + 1 - wc_word->length), &newError);
        if (newError) {
            exit(newError);
        }
    }
    strcpy(wc_word->word, word);
    *error = WCNoneError;
}

WCWordInfo wc_word_get_info(WCWord * word, WCError * error) {
    if (word == NULL) {
        *error = WCNullPointerError;
        WCWordInfo info;
        info.row = 0;
        info.column = 0;
        return info;
    }
    *error = WCNoneError;
    return word->info;
}

void wc_word_set_info(WCWord * word, WCWordInfo info, WCError * error) {
    if (word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    word->info = info;
    *error = WCNoneError;
}

int wc_word_get_length(WCWord * word, WCError * error) {
    if (word == NULL) {
        *error = WCNullPointerError;
        return 0;
    }
    *error = WCNoneError;
    return word->length;
}
