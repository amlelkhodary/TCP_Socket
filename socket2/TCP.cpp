#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <iostream>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define close closesocket
    #define SHUTDOWN SD_SEND
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif
using namespace std;
class TCPSocket
{
public:
    TCPSocket(std::string _addr, uint16_t _port){
        #ifdef _WIN32
        // Initialize Winsock
        int ret = WSAStartup(MAKEWORD(2, 2), &m_winsSocketInfo);
        if (ret != 0)
        {
            throw std::runtime_error("WSAStartup failed with error: " + to_string(WSAGetLastError()));
        }
    #else
        // No need to initialize Winsock for Linux
    #endif

        // Initialize address structure
        memset(&m_addr, 0, sizeof(m_addr));  // Use memset instead of ZeroMemory
        m_addr.sin_family = AF_INET;
        inet_pton(m_addr.sin_family, _addr.c_str(), &m_addr.sin_addr.s_addr);
        m_addr.sin_port = htons(_port);

        // Create socket
        m_socket = socket(m_addr.sin_family, SOCK_STREAM, IPPROTO_TCP);
        if (m_socket == -1)
        {
            throw std::runtime_error("Socket creation failed with error: " + to_string(errno));
        }
    }

    void Connect(){
        int ret = connect(m_socket, (struct sockaddr *)&m_addr, sizeof(m_addr));
        if (ret == -1)
        {
            throw std::runtime_error("Unable to connect to the server: " + to_string(errno));
        }
        m_isConnected = true;

        // Set socket receive timeout (platform-specific)
    #ifdef _WIN32
        DWORD timeout = 50;
        ret = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    #else
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 50000; // 50ms timeout
        ret = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    #endif

        // Enable keepalive (platform-specific)
    #ifdef _WIN32
        BOOL yes = 1;
        ret = setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&yes, sizeof(BOOL));
    #else
        int yes = 1;
        ret = setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
    #endif
    }

    void Disconnect(){
        if (!m_isConnected)
            return;

        #ifdef _WIN32
        // For Windows, shutdown with SD_SEND (send shutdown)
        int ret = shutdown(m_socket, SD_SEND);
        #else
            // For Linux, use SHUT_RDWR to disable both sending and receiving
            int ret = shutdown(m_socket, SHUT_RDWR);
        #endif
        if (ret == -1)
        {
            cout << "Shutdown failed with error: " + to_string(errno) << endl;
        }

        ret = close(m_socket);
        if (ret == -1)
        {
            cout << "Close failed with error: " + to_string(errno) << endl;
        }

        m_isConnected = false;
    }

    void Send(const std::vector<char>& _data){
        int ret = send(m_socket, _data.data(), _data.size(), 0);
        if (ret < 0)
        {
            throw std::runtime_error("Unable to send the data");
        }
    }

    bool IsConnected(){
        const int DEFAULT_BUFLEN = 800;
        char recvbuf[DEFAULT_BUFLEN] = "";
        int ret = recv(m_socket, recvbuf, DEFAULT_BUFLEN, 0);
        if (ret == 0)
        {
            return false;  // Connection closed by the server
        }
        return true;  // Still connected
    }

    std::vector<char> Receive(){
        const int DEFAULT_BUFLEN = 800;
        char recvbuf[DEFAULT_BUFLEN] = "";
        int ret = recv(m_socket, recvbuf, DEFAULT_BUFLEN, 0);
        if (ret == 0)
        {
            throw std::runtime_error("Connection closed by the server");
        }
        else if (ret < 0)
        {
            throw std::runtime_error("Unable to read the data");
        }
        else
        {
            std::vector<char> arr(recvbuf, recvbuf + ret);
            return arr;
        }
    }

    ~TCPSocket(){
    #ifdef _WIN32
        WSACleanup();
    #endif
    }
    
private:
    bool m_isConnected;
    struct sockaddr_in m_addr;
    #ifdef _WIN32
        SOCKET m_socket;
        WSADATA m_winsSocketInfo;
    #else
        int m_socket;
    #endif
};

#endif // TCPSOCKET_H
