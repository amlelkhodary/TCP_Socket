#include "server.h"
Server::Server(string _address, uint32_t _port, uint32_t clients_no){
    //Create the Socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if(server_socket == -1){
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

    // Bind the socket to the address
    if (bind(server_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket\n";
        closesocket(server_socket);
    }

    // Listen to the incoming connections
    if(listen(server_socket, clients_no) < 0){
        std::cerr << "Error listening on socket\n";
        closesocket(server_socket);
    }
    cout<<"The server is listening on port "<<_port<<endl;

    //Accept the connection request that is received on the socket the application was listening to
    int client_socket = accept(server_socket, nullptr, nullptr);
    if(client_socket < 0){
        cerr<<"Accept the connection failed with error "<< strerror(errno) << endl;
        closesocket(server_socket);
    }

    //Receive Data or Message from the client
    char buffer[1024] = {0};
    int data_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (data_received > 0) 
    {
        std::cout << "Message from client: " << buffer << std::endl;
    } 
    else if (data_received == 0) 
    {
        std::cout << "Connection closed by client." << std::endl;
    } 
    else 
    {
        std::cerr << "Recv failed with error: " <<strerror(errno) << std::endl;
    }

}