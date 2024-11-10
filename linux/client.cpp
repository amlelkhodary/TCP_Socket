// // Linux client program for socket programming
// #include <cstring>
// #include <iostream>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>

// int main() {
//     // Creating client socket
//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (clientSocket < 0) {
//         std::cerr << "Socket creation failed" << std::endl;
//         return 1;
//     }

//     // Specifying server address
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(9000);
//     // Replace with the Windows server's IP address
//     inet_pton(AF_INET, "172.19.0.208", &serverAddress.sin_addr); // Use the Windows server's IP

//     // Connecting to server
//     if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
//         std::cerr << "Connection failed" << std::endl;
//         close(clientSocket);
//         return 1;
//     }

//     // Sending data
//     const char* message = "Hello from Linux client!";
//     send(clientSocket, message, strlen(message), 0);

//     std::cout << "Message sent to server successfully!" << std::endl;

//     // Closing socket
//     close(clientSocket);

//     return 0;
// }


// Linux client program for socket programming
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    // Creating client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    // Specifying server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    // Replace with the Linux server's IP address
    inet_pton(AF_INET, "172.19.0.208", &serverAddress.sin_addr); // Use the Linux server's IP

    // Connecting to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Sending data
    const char* message = "Hello from Linux client!";
    send(clientSocket, message, strlen(message), 0);

    std::cout << "Message sent to server successfully!" << std::endl;

    // Closing socket
    close(clientSocket);

    return 0;
}
