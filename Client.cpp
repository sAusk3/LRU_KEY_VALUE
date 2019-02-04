#include <array>
#include <string>
#include <iostream>
#include <stdexcept>

#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 22000
#define MAX_SIZE 1024

class myclient
{
    int m_fd{};
    struct sockaddr_in m_addr{};

public:

    explicit myclient(uint16_t port)
    {
        if (m_fd = ::socket(AF_INET, SOCK_STREAM, 0); m_fd == -1) {
            throw std::runtime_error(strerror(errno));
        }

        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        m_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        if (connect() == -1) {
            throw std::runtime_error(strerror(errno));
        }
    }

    int connect()
    {
        std::cout << "Connection has been established " << std::endl;
        return ::connect(
            m_fd,
            reinterpret_cast<struct sockaddr *>(&m_addr),
            sizeof(m_addr)
        );
    }

    ssize_t send(const std::string &buf)
    {
        return ::send(
            m_fd,
            buf.data(),
            buf.size(),
            0
        );
    }

    ssize_t recv(std::array<char, MAX_SIZE> &buf)
    {
        return ::recv(
            m_fd,
            buf.data(),
            buf.size(),
            0
        );
    }

    void useKeyValueMap()
    {
        int choice = 0;
        while(true) {
            std::cout << " Choose any of the three options....[1,2,3,4]" << std::endl << std::endl;
            std::cout << " 1. PUT " << std::endl;
            std::cout << " 2. DELETE " << std::endl;
            std::cout << " 3. GET " << std::endl << std::endl;
            std::cout << " 4. PRINT CONTENTS OF HASH TABLE" << std::endl << std::endl;

            std::cout << " Enter your choice....: ";
            std::cin >> choice;
            
            std::string sendbuf{};
            std::array<char, MAX_SIZE> recvbuf{};
            std::string key{};
            std::string value{};
            std::pair<std::string, std::string> kvPair;
            switch (choice)
            {
                case 1:
                {   
                    std::cout << "Entering case 1" << std::endl;
                    sendbuf = "1";
                    send(sendbuf);
                    recv(recvbuf);
                    if(strcmp(recvbuf.data(),"OK1")==0)
                    {
                        std::fill(key.begin(),key.end(),'\0');
                        std::fill(value.begin(),value.end(),'\0');
                        std::cout<<" Enter the key(string) to be inserted: ";
                        std::cin.ignore();
                        std::getline(std::cin, key);
                        std::cout<<"Client : Key Value Entered is "<< key << std::endl;
                        send(key);
                        std::fill(recvbuf.begin(),recvbuf.end(),'\0');
                        recv(recvbuf);
                        if(strcmp(recvbuf.data(),"OK2")==0)
                        {
                            std::cout<<" Enter the Value(string) : ";
                            getline(std::cin, value);
                            std::cout<<"Client : Sending the value to server as "<<value<<std::endl;
                            send(value);
                            std::fill(recvbuf.begin(),recvbuf.end(),'\0');
                            recv(recvbuf);
                            if(strcmp(recvbuf.data(), "OK3")==0)
                            std::cout<<"Key: "<< key << " Value: "<< value << " has been successfully Inserted by the Server "<<std::endl;
                        }
                    }      
                break;
                }
                case 2:
                {
                    std::cout << "Entering case 2" << std::endl;
                    sendbuf = "2";
                    send(sendbuf);
                    recv(recvbuf);
                    std::fill(key.begin(),key.end(),'\0');
                    if(strcmp(recvbuf.data(),"OK1")==0)
                    {
                        std::cout<<" Enter the key(string) to be deleted : ";
                        std::cin.ignore();
                        std::getline(std::cin, key);
                        send(key);
                        std::fill(recvbuf.begin(),recvbuf.end(),'\0');
                        recv(recvbuf);
                        if(strcmp(recvbuf.data(),"OK2")==0)
                        {
                            std::cout<<"Key Value has been successfully received by the Server "<<std::endl;
                        }
                    }
                break;
                }
                case 3:
                {
                    std::cout << "Entering case 3" << std::endl;
                    sendbuf = "3";
                    send(sendbuf);
                    recv(recvbuf);
                    std::fill(key.begin(),key.end(),'\0');
                    if(strcmp(recvbuf.data(),"OK1")==0)
                    {
                        std::cout<<" Enter the key(string) to be searched for : ";
                        std::cin.ignore();
                        std::getline(std::cin, key);
                        send(key);
                        std::fill(recvbuf.begin(),recvbuf.end(),'\0');
                        recv(recvbuf);
                        if(strcmp(recvbuf.data(),"OK2")==0)
                        {
                            std::cout<<"Key has been successfully received by the Server "<<std::endl;
                        }
                        std::fill(recvbuf.begin(),recvbuf.end(),'\0');
                        recv(recvbuf);
                        std::cout<<"Value received from server is : "<< recvbuf.data() << std::endl;
                        if(strcmp(recvbuf.data(),"OK3")==0)
                            std::cout << "Value Not Found" << std::endl;
                        else
                            std::cout<<"Value Found : "<<recvbuf.data() << std::endl;
                    }
                break;
                }
                case 4:
                {
                    std::cout << "Entering case 4" << std::endl;
                    sendbuf = "4";
                    send(sendbuf);
                    break; 
                }
                default:
                std::cout<<"Do nothing"<<std::endl;
                exit(0);
            
            }

            // std::string sendbuf{};

            // std::array<char, MAX_SIZE> recvbuf{};

            // std::getline(std::cin, sendbuf);

            // send(sendbuf);
            // recv(recvbuf);

            // std::cout << "client receives "<<recvbuf.data() << '\n';
        }
    }

    ~myclient()
    {
        close(m_fd);
    }
};

int main(int argc, char* argv[])
{
    myclient client{PORT};
    client.useKeyValueMap();

    return EXIT_SUCCESS;
}
