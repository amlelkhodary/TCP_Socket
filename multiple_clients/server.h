#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstdint>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")

class Server{
    private:
    int server_socket ;
    struct sockaddr_in serverAddress;
    public:
    Server(uint32_t port, uint32_t clients_no);
};
#endif