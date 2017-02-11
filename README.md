# WordCounter

## Introduction

This is the course design of Data Structure from Computer Science and Technology Department in HUST.

A word counter program based on Trie Tree and Hash Table.

The author of this program - BlackDragon - is an undergraduate of HUST. 

Any discussion or suggestion is welcomed, but you SHOULD NOT COPY the code directly if you are facing the same/similar course design. After all, copying the codes won't make you stronger :)

## Architecture

The program can be devided into these parts -

* WCWord - The abstraction of English words, essentially a variable-length sequence list.
* WCIndex - The abstraction of indexes for a certain word, essentially a linked list with a head node, has iterator API for traversal.
* WCWordExtractor - The abstraction of the easiest state machine, which extracts words from char buffer.MIND the extractor is case insensitive and could not parse words with character **-** or **'** like *hard-working* or *don't*.
* WCTrieTree - The naive implementation of trie tree.
* WCHashTable - The implementation of hash table, using RSHash as hash function, has iterator API for traversal.
* WCFileHandler - The wrapper for FILE pointer, managing file I/O and the serialization of data structure.
* WCErrorHandler - The definition for possible error types.                                                                                                                                                                                             

## Shortcoming & Improving

* need a better way of error handling, in the current circumstance, program will exit directly if an error is detected.
* the serialization of trie tree remains to be done.
* trie tree can be traversed with the help of iterator.
* need a better method to extract words.
* the naive implementation of trie tree occupies much memory, which could be improved by using knowledge from compilers.
* multithread programming can be used to improve the performance of the program.

## Log

* 2017/01/14 initial commit
* 2017/01/15 design general architecture of program
* 2017/02/09 finish all basic modules
* 2017/02/11 extra: serialize and deserialize of hash table

## License

MIT License(c) BlackDragon



