#include "../../include/client/client.hpp"
#include <iostream>

using namespace Thesisuis;

int main() {
    Client client;
    int clientSocket = client.createSocket();
    sockaddr_in serverAddress = client.getServerAddress();
    client.connectToServer(clientSocket, serverAddress);
    client.signin(clientSocket);
    std::string msg;
    std::cout << ">> ";
    std::getline(std::cin, msg);
    while (msg != "exit") {
        const char* message = msg.c_str();
        client.sendData(clientSocket, message);
        std::cout << ">> ";
        std::getline(std::cin, msg);
    }
}
