#include "server.h"
Server::Server(uint32_t _port, uint32_t clients_no){
    //Create the Socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if(server_socket == -1){
        cerr<<"socket creation failed with "<< strerror(errno) << endl; //Access the last error in C++
    }

    // Setting up address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(_port);

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