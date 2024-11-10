// Linux server program for socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    // Creating server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Binding socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Bind failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Listening to the assigned socket
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Listen failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server is listening on port 8080..." << std::endl;

    // Accepting connection request
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        std::cerr << "Accept failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Receiving data
    char buffer[1024] = { 0 };
    int recvResult = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (recvResult > 0) {
        std::cout << "Message from client: " << buffer << std::endl;
    } else if (recvResult == 0) {
        std::cout << "Connection closed by client." << std::endl;
    } else {
        std::cerr << "Recv failed" << std::endl;
    }

    // Closing sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
