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
    while (true) {
        std::cout << "#> ";
        std::getline(std::cin, msg);
        client.sendData(clientSocket, msg);
    }
}
