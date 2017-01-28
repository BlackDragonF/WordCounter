#include "WCTrieTree.h"
#include "WCIndex.h"
#define MAX_CHARACTER_NUMBER (26)

struct WCTrieNode {
    char character;
    struct WCTrieNode * child[MAX_CHARACTER_NUMBER];
    WCIndex * index;
}

struct WCTrieTree {
    struct WCTrieNode * root;
};
