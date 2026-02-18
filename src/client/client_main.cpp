#include "../../include/client/client.hpp"
#include "../../include/utils.hpp"
#include "../../include/socket_guard.hpp"
#include <iostream>

using namespace Thesisuis;

int main() {
    Client client;
    SocketGuard clientSocket(client.createSocket());
    sockaddr_in serverAddress = client.getServerAddress();
    client.connectToServer(clientSocket.get(), serverAddress);
    client.signin(clientSocket.get());
    while (true) {
        std::cout << "#> ";
        std::string cmd {};
        std::getline(std::cin, cmd);
        sendData(clientSocket.get(), cmd);
        if (cmd == "add_user") client.signin(clientSocket.get());
        else if (cmd == "exit") break;
        std::string response = receiveData(clientSocket.get());
        std::cout << response << std::endl;
    }
    return 0;
}
