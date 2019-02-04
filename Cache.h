#ifndef HASHTABLE_CACHE_H
#define HASHTABLE_CACHE_H


#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <list>
#include <unordered_map>

class Cache {
    typedef std::pair<std::string,std::string> cacheEntry_;
    typedef std::list<cacheEntry_> cacheList_;
    typedef std::unordered_map<std::string, cacheList_::iterator> cacheMap_;

    public:
    Cache(int size);//max size of cache
    void Delete(const std::string key);
    void Put(const std::string key, const std::string value);
    std::string Get(const std::string key);
    void Print();
    
    void input(cacheEntry_ entry);
    void fileFind(const std::string key);
    std::string GetValueFromFile(std::string key);
    ~Cache();

    private:
    int maxSize;
    cacheEntry_ cacheEntry;
    cacheList_ cacheList;
    cacheMap_ cacheMap;
    void removeTail();
};

#endif