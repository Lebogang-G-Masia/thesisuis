#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "../../include/client/client.hpp"
#include "../../include/utils.hpp"

using namespace Thesisuis;

void Client::signin(int clientSocket) {
    std::cout << "Enter username." << std::endl;
    std::cout << ">> "; 
    std::string username = userInput();
    std::cout << "Enter password." << std::endl;
    std::cout << ">> ";
    std::string password = userInput(true);
    std::cout << std::endl;
    sendData(clientSocket, username);
    sendData(clientSocket, password);
}

int Client::createSocket() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create client socket" << std::endl;
        exit(1);
    }

    return clientSocket;
}

sockaddr_in Client::getServerAddress() {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    // convert the IP to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) == -1) {
        std::cerr << "Invalid IP or IP address not supported" << std::endl;
        exit(1);
    }

    return serverAddress;
}

void Client::connectToServer(int clientSocket, sockaddr_in serverAddress) {
    std::cout << "[ ~ ] Connecting to server." << std::endl;
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Connection Failed" << std::endl;
        exit(1);
    }
    std::cout << "[ + ] Connected successfully" << std::endl;
}

