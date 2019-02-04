#include<iostream>

void openFile()
{
    std::ofstream ofile("store.bin", std::ios::binary | ios::in | ios::out );
    if(!ofile.is_open())
        {
          std::cout << "file could not be opened" << std::endl;
        }
}

void FindInFile(std::string filename, string key)

void WriteFile()



