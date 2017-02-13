# WordCounter

## Description

This is the course design of Data Structure from Computer Science and Technology Department in HUST.

A command-line word counter program based on Trie Tree and Hash Table.

The author of this program - BlackDragon - is an undergraduate of HUST. 

Any discussion/suggestion/issues/pull requests is welcomed, but you SHOULD NOT COPY the codes directly if you are facing the same/similar course design/experiment. After all, copying the codes won't make you stronger :)

`wordcounter [OPTIONS] file [FILENAME]`

## Options
	
	-h				Print this help text and exit
	-d				Use data file(hash table) instead of text file to generate look-up table
	-s				Data structure used to generate look-up table - trie, hash, both - default is both
					If -d is specified, -s will be ignored
	-k				Save the look-up table(hash table) to file
					If -s isn't specified with hash or both, -k will be ignored
	-f				Enable interactive find mode
	-t				Print all the words and their positions in text


## Examples

	# Print the help text
	$ wordcounter -h
	
	# Generate the look-up table using hash table and trie tree both
	$ wordcounter -s both text.txt
	
	# Generate the look-up table using hash table and save to file
	$ wordcounter -s hash -k text.txt
	
	# Load the look-up table(hash table) from file and traverse
	$ wordcounter -dt data.dat
	
	# Generate the look-up table using trie tree and search words
	$ wordcounter -s trie -f text.txt

## Architecture

The program can be devided into these parts -

* WCWord - The abstraction of English words, essentially a variable-length sequence list.
* WCIndex - The abstraction of indexes for a certain word, essentially a linked list with a head node, has iterator API for traversal.
* WCWordExtractor - The abstraction of the easiest state machine, which extracts words from char buffer.MIND the extractor is case insensitive and could not parse words with character **-** or **'** like *hard-working* or *don't*.
* WCTrieTree - The naive implementation of trie tree.
* WCHashTable - The implementation of hash table, using RSHash as hash function, has iterator API for traversal.
* WCFileHandler - The wrapper for FILE pointer, managing file I/O and the serialization of data structure.
* WCErrorHandler - The definition for possible error types. 
* WCClock - The abstraction of clock_t and clock() in *time.h*, providing function of counting time interval.
* WordCounter - The main entry and the implementation for each function using basic modules.                                                                                                                                                                                            

## Shortcoming & Improving

> It is all welcomed that you fork this project and continue with this program and pull requests are welcomed as well :)

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
* 2017/02/12 add WCClock, finish command-line program and debug
* 2017/02/13 add unique count

## License

MIT License(c) BlackDragon



