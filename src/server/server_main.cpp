#include "../../include/server/server.hpp"
#include "../../include/socket_guard.hpp"
#include <iostream>
#include <stdexcept>

using namespace Thesisuis;

int main() {
    try {
        Server server;
        SocketGuard serverSocket(server.createSocket());
    
        server.bindSocket(serverSocket.get());
        server.listenForConnection(serverSocket.get());
    
        std::cout << "Waiting for connections..." << std::endl;
    
        while (true) {
            try {
                SocketGuard clientSocket(server.acceptConnection(serverSocket.get()));
                std::cout << "Client connected." << std::endl;
    
                if (!server.authenticate(clientSocket.get())) {
                    std::cerr << "Authentication Failed" << std::endl;
                    continue;
                }
                std::cout << "Authentication successful" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error handling client: " << e.what() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal server error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
