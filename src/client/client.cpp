#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../include/utils.hpp"

int main() {
    // create the socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "ERROR: Failed to create the client socket." << std::endl;
        crash();
    }

    // specify the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        std::cerr << "ERROR: Invalid IP address or IP address not supported." << std::endl;
        crash();
    }

    // connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cout << "ERROR: Connection Failed." << std::endl;
        crash();
    }

    // send data
    const char* message = "Hello from the client.";
    send(clientSocket, message, strlen(message), 0);

    // receive data
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "The server says: " << buffer << std::endl;


    // close the socket
    close(clientSocket);

    return 0;
}
