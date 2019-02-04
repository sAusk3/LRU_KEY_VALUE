#include "Cache.h"
#include <iostream>
#include <string>

int main()
{
    //Create a cache of size 10
    Cache lru(10);
   
    std::cout << "Testing Insertions into the Key-Value Store" << std::endl;
    //testing PUT
    lru.Put("0", "Zero");
    lru.Put("1", "one");
    lru.Put("2", "two");
    lru.Put("3", "three");
    lru.Put("4", "four");
    lru.Put("5", "five");


    std::cout << std::endl << "Testing Print() function" << std::endl;
    //testing if the printint the list works
    lru.Print();

    std::cout << "Trying to overwrite an existing keys 1 and 2 to Uppercases" << std::endl;
    //testing if the same key is over written
    lru.Put("1", "ONE");
    lru.Put("2", "TWO");

    //Validating the results by printing it again
    std::cout << std::endl << "Validating the results by printing it again" << std::endl;
    lru.Print();

     // testing the getfromthefile functionality 
     std::cout << std::endl << "Testing the getfromthefile functionality" << std::endl;
     std::string testKey = "2";
     std::string testValue = lru.GetValueFromFile(testKey);
     std::cout << "Value found for " << testKey << " is " << testValue << std::endl;

     //testing the Get Function
     std::cout << std::endl << "Testing the Get Function" << std::endl;
     testKey = "2";
     testValue = lru.Get(testKey);
     if(testValue.empty())
        std::cout << "Value not found" << std::endl;
     else
        std::cout << "Value found for " << testKey << " is " << testValue << std::endl;

    std::cout << "Testing the delete() of key 1 from Cache" << std::endl;
    //testing if delete works
    lru.Delete("1");
    lru.Print();

    std::cout << std::endl << "Trying to delete(6) an entry which is not there" << std::endl;
    //testing if we try to delete a key which is not present
    lru.Delete("6");

    std::cout << std::endl << "Trying the cache eviction by filling up the Cache" << std::endl;
    //testing the cache eviction as till now the cache has 4 elements
    lru.Put("6", "six");
    lru.Put("7", "seven");
    lru.Put("8", "eight");
    lru.Put("9", "nine");
    lru.Put("10", "ten");
    std::cout << std::endl << "Cache should get full by now" << std::endl;
    lru.Print();

    lru.Put("11", "eleven");
    std::cout << std::endl << "Cache eviction should happen by inserting 11" << std::endl;
    lru.Print();

    //todo now find the evicted item from the file
    testKey = "1";
    testValue = lru.Get("1");
    if(testValue.empty())
        std::cout << "Value not found" << std::endl;
    else
        std::cout << "Value found for " << testKey << " is " << testValue << std::endl;

    std::cout << std::endl << "look for a value 12 which is not there" << std::endl;
    testValue = lru.Get("12");
    if(testValue.empty())
        std::cout << "Value not found" << std::endl;
    else
        std::cout << "Value found for " << testKey << " is " << testValue << std::endl;
    
}