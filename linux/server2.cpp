#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>  // For multithreading (optional for handling multiple clients concurrently)

void handleClient(int clientSocket) {
    // Receiving and handling client messages
    char buffer[1024] = { 0 };
    while (true) {
        int recvResult = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvResult > 0) {
            std::cout << "Message from client: " << buffer << std::endl;
            // Optionally, send a response back to the client
            send(clientSocket, "Message received", 16, 0);
        } else if (recvResult == 0) {
            std::cout << "Client disconnected." << std::endl;
            break;  // Break the loop if client disconnects
        } else {
            std::cerr << "Recv failed" << std::endl;
            break;
        }
    }

    // Closing the client socket
    close(clientSocket);
}

int main() {
    // Creating server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set the SO_REUSEADDR option to allow immediate reuse of the port
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Failed to set SO_REUSEADDR option.\n";
        close(serverSocket);
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

    // Accepting connections in a loop to keep the server running
    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            std::cerr << "Accept failed" << std::endl;
            continue;  // Keep the server running if a connection attempt fails
        }

        std::cout << "Client connected." << std::endl;

        // Optionally, handle each client in a separate thread to allow concurrent connections
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();  // Detach the thread to let it run independently
    }

    // Closing the server socket (this line will never be reached due to the infinite loop)
    close(serverSocket);

    return 0;
}
