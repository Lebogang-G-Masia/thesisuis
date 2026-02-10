#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include "../../include/client/client.hpp"
#include "../../include/utils.hpp"

using namespace Thesisuis;

std::vector<const char*> Client::signin() {
    std::cout << "Enter username." << std::endl;
    std::cout << ">> " << std::endl;
    std::string username = userInput();
    std::string password = userInput(true);
    const char* uname = username.c_str();
    const char* psswd = password.c_str();
    std::vector<const char*> creds = { uname, psswd};
    return creds;
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

void Client::sendData(int clientSocket, const char* msg) {
    send(clientSocket, msg, strlen(msg), 0);
}

void Client::closeSocket(int clientSocket) {
    close(clientSocket);
}
