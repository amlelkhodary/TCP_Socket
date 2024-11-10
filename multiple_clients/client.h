#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstdint>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

class Client{
    public:
        Client(string _address, uint32_t _port);
        void send_message(const char* input);
    private:
        int client_socket = 0;
};
#endif