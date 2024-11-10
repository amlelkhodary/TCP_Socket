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

class TCPSocket
{
public:
    TCPSocket(std::string _addr, uint16_t _port);
    void Connect();
    void Disconnect();
    void Send(const std::vector<char>& _data);
    bool IsConnected();
    std::vector<char> Receive();
    ~TCPSocket();
    
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
