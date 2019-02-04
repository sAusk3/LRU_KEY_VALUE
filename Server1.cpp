#include <iostream>
#include <pthread.h>
#include <mutex>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <thread>
#include <functional>
#include <type_traits>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <array>
#include <stdexcept>
#include <string.h>
#include "Cache.h"


#define PORT 22000
#define MAX_SIZE 1024

Cache lru(10);

class myserver
{
    int m_fd{};
    int m_client{};
    struct sockaddr_in m_addr{};


public:

    explicit myserver(uint16_t port)
    {
        // TCP/IP connection IP4 connection returns the socket file descriptor
        if (m_fd = ::socket(AF_INET, SOCK_STREAM, 0); m_fd == -1) {
            throw std::runtime_error(strerror(errno));
        }
        else
            std::cout << "TCP/IP4 Socket successfully created on Server." << std::endl;
        
        //AF_INET for IP4
        m_addr.sin_family = AF_INET;
        // htons converts the port number to network byte order
        m_addr.sin_port = htons(port);
        //accepts connection from any IP addresses
        m_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (this->bind() == -1) {
            throw std::runtime_error(strerror(errno));
        }
        else
            std::cout << "Socket Binding done successfully." << std::endl;
    }

    int bind()
    {
        return ::bind(
            m_fd,
            reinterpret_cast<struct sockaddr *>(&m_addr),
            sizeof(m_addr)
        );
    }

    ssize_t recv(std::array<char, MAX_SIZE> &buf)
    {
        return ::recv(
            m_client,
            buf.data(),
            buf.size(),
            0
        );
    }

    ssize_t send(std::string &buf)
    {

        return ::send(
            m_client,
            buf.data(),
            buf.size(),
            0
        );
    }

    int listenAccept()
    {
        if (::listen(m_fd, 0) == -1) {
            throw std::runtime_error(strerror(errno));
        }
        else {
            std::cout << "Socket is listening to incoming connections." << std::endl;
        }
        
        socklen_t csize=sizeof(m_addr);
        m_client = ::accept(m_fd, reinterpret_cast<struct sockaddr *>(&m_addr), &csize);
        if (m_client == -1) {
            throw std::runtime_error(strerror(errno));
        }
        else {
            std::cout << "Accepted connection from client : " << inet_ntoa(m_addr.sin_addr)<< std::endl;
            return m_client;
        }
        return 0;
    }
    
    

    ~myserver()
    {
        close(m_fd);
    }
};

void* useKeyValueMap(void* newSock)
{
    int newSocket = *(int*)newSock;
    std::array<char, MAX_SIZE> buf{};
    std::array<char, MAX_SIZE> choice{};
    std::array<char, MAX_SIZE> key{};
    std::array<char, MAX_SIZE> value{};
    std::mutex lock_1;

    while(true)
    {
        lock_1.try_lock();
        ::recv(newSocket, choice.data(), choice.size(), 0);
        if(strcmp(choice.data(),"1")==0)
        {
            std::fill(key.begin(),key.end(),'\0');
            std::fill(value.begin(),value.end(),'\0');
            std::cout << "Server received the choice to PUT() " << choice.data() << std::endl;
            std::string s1 {"OK1"};
            ::send(newSocket, s1.data(), s1.size(), 0);
                ::recv(newSocket, key.data(), key.size(), 0);
                std::cout << "Server received the key value " << key.data() << std::endl;
                std::string s2 {"OK2"};
                ::send(newSocket, s2.data(), s2.size(), 0);
                recv(newSocket, value.data(), value.size(), 0);
                std::cout << "Server received the Value : " << value.data() << std::endl;
                std::string s3 {"OK3"};
                ::send(newSocket, s3.data(), s3.size(), 0);
                lru.Put(key.data(), value.data());
            }
            else if(strcmp(choice.data(),"2")==0)
            {
                std::fill(key.begin(),key.end(),'\0');
                std::cout << "Server received the choice to Delete() " << choice.data() << std::endl;
                std::string s1 {"OK1"};
                ::send(newSocket, s1.data(), s1.size(), 0);
                ::recv(newSocket, key.data(), key.size(), 0);
                std::cout << "Server received the key value " << key.data() << std::endl;
                std::string s2 {"OK2"};
                ::send(newSocket, s2.data(), s2.size(), 0);
                lru.Delete(key.data());
            }

            else if(strcmp(choice.data(),"3")==0)
            {
                std::fill(key.begin(),key.end(),'\0');
                std::cout << "Server received the choice to GET() " << choice.data() << std::endl;
                std::string s1 {"OK1"};
                ::send(newSocket, s1.data(), s1.size(), 0);
                ::recv(newSocket, key.data(), key.size(), 0);
                std::cout << "Server received the key value " << key.data() << std::endl;
                std::string s2 {"OK2"};
                ::send(newSocket, s2.data(), s2.size(), 0);
                std::string result {""};
                result = lru.Get(key.data());
                std::string s3 {"OK3"};
                std::cout << "Server found the result as : " << result << std::endl;
                if(result.empty())
                    ::send(newSocket, s3.data(), s3.size(), 0);
                else
                    ::send(newSocket, result.data(), result.size(), 0);
            }

            else if(strcmp(choice.data(),"4")==0)
            {
                std::cout << "Server received the choice to Print() " << choice.data() << std::endl;
                lru.Print();
            }
            else 
            {
                std::cout << "Receiving incorrect choices" << std::endl;
                break; 
            }
            lock_1.unlock();
    }
        
    close(newSocket);
    return nullptr;
}

int protected_main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    myserver server{PORT};
    std::vector<pthread_t> T;
    while(int fd = server.listenAccept())
    {
        pthread_t threadID;
        T.push_back(threadID);
        pthread_create(&threadID, NULL, useKeyValueMap , (void*) &fd);
    }
    
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    try {
        return protected_main(argc, argv);
    }
    catch (const std::exception &e) {
        std::cerr << "Caught unhandled exception:\n";
        std::cerr << " - what(): " << e.what() << '\n';
    }
    catch (...) {
        std::cerr << "Caught unknown exception\n";
    }

    return EXIT_FAILURE;
}