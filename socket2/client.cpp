#include "TCPSocket.h"
#include <iostream>
#include <vector>
#include "TCPSocket.h"

int main()
{
    try
    {
        // Define the server address and port
        std::string serverAddress = "127.0.0.1";  // Replace with server IP
        uint16_t serverPort = 8080;  // Replace with server port

        // Create the TCP socket and connect to the server
        TCPSocket socket(serverAddress, serverPort);
        socket.Connect();
        std::cout << "Connected to server at " << serverAddress << ":" << serverPort << std::endl;

        // Prepare data to send
        std::vector<char> dataToSend = {'H', 'e', 'l', 'l', 'o', ' ', 'S', 'e', 'r', 'v', 'e', 'r'};
        socket.Send(dataToSend);
        std::cout << "Data sent to server." << std::endl;

        // Receive response from the server
        std::vector<char> receivedData = socket.Receive();
        std::cout << "Received data from server: ";
        for (char c : receivedData)
        {
            std::cout << c;
        }
        std::cout << std::endl;

        // Close the connection
        socket.Disconnect();
        std::cout << "Disconnected from server." << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
