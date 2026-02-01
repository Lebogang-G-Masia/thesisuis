#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../include/utils.hpp"


int main() {
    // create the socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "ERROR: Failed to create server socket." << std::endl;
        crash();
    }

    // bind the socket to an IP and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "ERROR: Failed to bind the socket." << std::endl;
        crash();
    }

    // listen
    if (listen(serverSocket, 5) == -1) { // Set a backlog of 5 connections
        std::cerr << "ERROR: Failed to listen." << std::endl;
        crash();
    }

    std::cout << "Listening on port 8080..." << std::endl;

    // accept connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        std::cerr << "ERROR: Failed to accept connection." << std::endl;
    }

    // receive data
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "The client says: " << buffer << std::endl;

    // send data
    const char* message = "Hello from the server!!";
    send(clientSocket, message, strlen(message), 0);

    // close the sockets
    close(serverSocket);
    close(clientSocket);

    return 0;
}
