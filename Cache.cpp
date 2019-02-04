#include "Cache.h"
#include <climits>
#include <fstream>

void Cache::input(cacheEntry_ entry)
{
    std::ofstream file_obj;
    //append mode so that the file is written at the end
    file_obj.open("KeyValueStore.txt", std::ios::app);
    std::string key = entry.first;
    std::string value = entry.second;
    file_obj << key <<std::endl;
    file_obj << value <<std::endl;
    file_obj.close();
}

void Cache::fileFind(std::string key)
{
    std::ifstream file_obj;
    file_obj.open("KeyValueStore.txt", std::ios::in);
    cacheEntry_ entry{};
    std::string temp{};
    std::string value{};

    std::cout << "Trying to find the key in disk " << std::endl;

    while(getline(file_obj,temp))
    {
        if(temp.compare(key))
            getline(file_obj,value);
        else
            getline(file_obj,temp);
    }

    if(!value.empty())
        std::cout << "Key "<< key << " Value is " << value << std::endl;
    else
        std::cout << "Not Found in File." << std::endl;
    file_obj.close();
}

std::string Cache::GetValueFromFile(std::string key)
{
    std::ifstream file_obj;
    file_obj.open("KeyValueStore.txt", std::ios::in);
    cacheEntry_ entry{};
    std::string temp{};
    std::string value{};

    std::cout << "Trying to find the key in disk " << std::endl;

    while(getline(file_obj,temp))
    {
        if(temp == key)
            getline(file_obj,value);
        else
            getline(file_obj,temp);
    }

    if(!value.empty())
        return value;
    else
        return {};
}

Cache::Cache(int size):maxSize(size)
{
    cacheMap.reserve(size);

}


// Employs the write through policy and inserts every entry in the file
// @params string : key 
// @params string : value
// @return void
// as the key-value is inserted in it's correctful position in the file and the list
void Cache::Put(const std::string key, const std::string value)
{
    cacheEntry_ entry = std::make_pair(key,value);
    input(entry);

    //if the size is full then follow LRU eviction policy
    //No need to evict into file due to write through policy
    if(cacheMap.size() >= maxSize )
        removeTail();
    
    auto mapIt = cacheMap.find(key);
    if(mapIt == cacheMap.end()) {
        //key is not found and a new key is being inserted at the head
        auto listIt = cacheList.insert(cacheList.begin(),std::make_pair(key, value));
        cacheMap.emplace(key, listIt);
    } 
    else {
        //key is found so overwrite the existing value and put it at the head
        auto& listIt = mapIt->second;
        
        // O(1) get the position of the key-Value in the list
        cacheEntry_ entry = *listIt;
        entry.second = value;

        //Removing the existing entry from the list
        cacheList.erase(listIt);
        cacheMap.erase(mapIt);

        cacheList.push_front(entry);
        cacheMap.emplace(key, cacheList.begin());
    }
}

// Deletes the key if found in the list
// Does not do file operation as the file is supposed to persist the key value
// @params string : key 
// @return void
// as the key-value are deleted from their positions in the list and map
void Cache::Delete(std::string key)
{
    if(cacheMap.empty()) {
        std::cout<<"Cache is empty so no deletion is possible"<<std::endl;
        return;
    }
    auto mapIt = cacheMap.find(key);
    if(mapIt == cacheMap.end()) {
        std::cout<<"Key not found"<<std::endl;
        return;
    }
    auto listIt = mapIt->second;
    cacheMap.erase(mapIt);
    cacheList.erase(listIt);
}

// Get()
// @Params string : key 
// key is the key of the value to be searched in the cache and store
// @return string value
// empty string if key is not found in file or cache
// value of the corresponding key if found in the file
std::string Cache::Get(const std::string key )
{
    auto mapIt = cacheMap.find(key);
    std::string result {};
    if(mapIt != cacheMap.end()) {
        std::cout << "Key found in cache "<<std::endl; 
        auto& listIt = mapIt->second;
        return result = listIt->second;
    }
    else {
        std::cout << "Key Not found in cache "<<std::endl; 
        result = GetValueFromFile(key);
        if(!result.empty())
            std::cout << "Key found in file "<<std::endl; 
        return result;
    }
}
    

void Cache::Print()
{
    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;
    
    for(const auto& item : cacheList)
        std::cout<<item.first <<" : "<<item.second <<std::endl;
    
    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;
}

// removeTail()
// LRU eviction policy of removing the tail element which is the least recently used form the list
void Cache::removeTail()
{
    if(cacheMap.empty()) {
        std::cout<<"Cache is empty so no deletion is possible"<<std::endl;
        return;
    }
    cacheEntry_ lastEntry = cacheList.back();
    cacheMap.erase(lastEntry.first);
    cacheList.pop_back();
}

Cache::~Cache(){}