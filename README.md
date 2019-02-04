# Project Description
A key_value cache store needs to be implemented which supports the following:
a. Keys and Values should be strings of variable length
b. A cache eviction strategy needs to be implemented (LRU is chosen currently)
c. The program should be executed on a Client-Server Architecture
d. There should be a support for multiple clients
e. Limited memory storage
f. Cache eviction to a persistent storage ( currently a file system)

## Supported Operations
Key_Value store which supports the following functionalities:
a. PUT - inserts the key-value into the cache
b. DELETE - Deletes the key-value from the cache
c. GET - Gets the Value upon receiving the key as input
d. PRINT - Prints the current status of the key-value store

## File Descriptions
```
* Cache.h - header file for the Cache
* Cache.cpp - Implementation of the Cache member functions
* CacheClient.cpp - Contains static test cases to invoke the various functions to do the unit testing
* Client.cpp - contains the client code to receive the user inputs for the socket programming, looking for the server using TCP/IP4
* Server.cpp - Contains the server code for accepting a single client request
* Server1.cpp - Contains the server code to support multiple clients and is multithreaded.
* KeyValueStore.txt - Store the keys and Values in a text file to read/write from disk
```
## Build Instructions
```
g++ -std=c++17 Server.cpp Cache.cpp -o server
g++ -std=c++17 Server1.cpp Cache.cpp -o serverMultiple
g++ -std=c++17 Client.cpp -o client
```

## Authors

* **Rakesh Singh**
