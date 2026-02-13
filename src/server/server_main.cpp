#include "../../include/server/server.hpp"
#include <iostream>

using namespace Thesisuis;

int main() {
    Server server;
    int serverSocket = server.createSocket();
    sockaddr_in serverAddress = server.bindSocket(serverSocket);
    server.listenForConnection(serverSocket);
    int clientSocket = server.acceptConnection(serverSocket);
    bool auth = server.authenticate(clientSocket);
    if (!auth) {
        std::cerr << "Failed to authenticate" << std::endl;
        server.closeSockets(serverSocket, clientSocket);
        exit(2);
    }
    std::cout << "Authentication successfull" << std::endl;
    std::string msg = server.receiveData(clientSocket);
    while (msg != "exit") {
        std::cout << msg << std::endl;
        msg = server.receiveData(clientSocket);
    }
    server.closeSockets(serverSocket, clientSocket);
    return 0;
}
