#include "client.h"
Client::Client(string _address, uint32_t _port){
    //Create the Socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if(client_socket == -1){
        cerr<<"socket creation failed with "<< strerror(errno) << endl; //Access the last error in C++
    }

    // Specify the address
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));   //initialize the structure to zero
    serverAddress.sin_family = AF_INET; //AF_INET specifies the IPv4 protocol family
    serverAddress.sin_port = htons(_port);
    
    // Convert the IP address from string to the binary format required by sin_addr
    // without c_str() ---> error: no suitable conversion function from "std::string" to "PCSTR" (aka "const CHAR *") exists
    if(inet_pton(AF_INET, _address.c_str(), &serverAddress.sin_addr) <0 ){
        cerr<<"Invalid Address\n";
    }

    // Sending connection request to the server
    if (connect(client_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Connection failed with error: " << strerror(errno) << std::endl;
        closesocket(client_socket);
    }
}
void Client::send_message(const char* input){
    // Sending data from the client to the server
    int sendResult = send(client_socket, input, strlen(input), 0);
    if (sendResult < 0) {
        std::cerr << "Send failed with error: " << strerror(errno) << std::endl;
        closesocket(client_socket);
    }
}