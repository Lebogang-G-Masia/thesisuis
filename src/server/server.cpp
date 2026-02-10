#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "../../include/server/server.hpp"

using namespace Thesisuis;

int Server::createSocket() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create the server socket" << std::endl;
        exit(1);
    }
    return serverSocket;
}

sockaddr_in Server::bindSocket(int serverSocket) {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind the socket" << std::endl;
        exit(1);
    }
    return serverAddress;
}

void Server::listenForConnection(int serverSocket) {
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Failed to listen for a connection" << std::endl;
        exit(1);
    }
    std::cout << "Listening on port 8080..." << std::endl;
}

int Server::acceptConnection(int serverSocket) {
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept connection" << std::endl;
        exit(1);
    }
    return clientSocket;
}

char* Server::receiveData(int clientSocket) {
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    return buffer;
}

void Server::closeSockets(int serverSocket, int clientSocket) {
    close(serverSocket);
    close(clientSocket);
}
