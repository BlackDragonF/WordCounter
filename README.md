# WordCounter

## Introduction

The course design of Data Structure from Computer Science and Technology Department in HUST.

A word counter program based on Trie Tree and Hash Table.

The programmer of this program - BlackDragon - is an undergraduate of HUST. 

Any discussion or suggestion is welcomed, but you SHOULD NOT copy the code directly if you are facing the same/similar course design. After all, copying the codes won't make you stronger :)

## Ideas

The program will be devided into two parts - backend and frontend.

Backend will provide the core implementation of counting word, including pretreatment of text files, the initialization and management of certain data structures, and wrapping/abstraction api.

Frontend will provide the external interaction for the program.

The separation of the frontend and backend is good for managing the code of the program.

### Backend

#### Initialization and Destruction

The basis and perhaps the most important part of this program.

##### Initialization

The whole procedure of initialization can be devided into two steps - Pretreatment and Construction.

###### Pretreatment

Pretreatment aims at extracting words from given text file and generate WCWord structure variables with every word in the text file. 

###### Construction

Construction aims at using WCWord structure variables to construct/update Trie tree or Hash table and then append WCIndex node to certain linked lists.

##### Destruction

Destruction aims at freeing the related data structure safely and efficiently.

#### Management

The whole procedure of management mainly including querying from search tables althrough some operations such as 'delete' or 'insert' may be regarded as part of management as well.

#### Serialization and Deserialization

Serialization and deserialization mainly aims at saving and restoring the constructed search table in a fast way.
