#include "server.h"
#include "client.h"
#include <thread>
using namespace std;
void  startServer(){
    Server server(8080,10);
}
void startClient(){
    Client client1("127.0.0.1",8080);
    client1.send_message("Hello, I'm the client. My name is Amal");
}
int main(){
    // Initialize Winsock
    WSADATA wsaData;
    int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsResult != 0) {
        std::cerr << "WSAStartup failed: " << wsResult << std::endl;
        return 1;
    }

    thread server_thread(startServer);
    this_thread::sleep_for(chrono::seconds(1));
    thread client_thread(startClient);
    server_thread.join();
    client_thread.join();
    return 0;
}
