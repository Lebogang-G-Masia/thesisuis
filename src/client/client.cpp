#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <termios.h>
#include "../../include/client/client.hpp"

using namespace Thesisuis;

std::string Client::userInput(bool hide) {
    std::string input {};

    if (hide) {
       termios oldt;
       tcgetattr(STDIN_FILENO, &oldt);
       termios newt = oldt;
       newt.c_lflag &= ~ECHO;
       tcsetattr(STDIN_FILENO, TCSANOW, &newt);
       std::getline(std::cin, input);
       tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
       return input;
    }
    std::getline(std::cin, input);
    return input;
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
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Connection Failed" << std::endl;
        exit(1);
    }
}

void Client::sendData(int clientSocket, const char* msg) {
    send(clientSocket, msg, strlen(msg), 0);
}

void Client::closeSocket(int clientSocket) {
    close(clientSocket);
}
