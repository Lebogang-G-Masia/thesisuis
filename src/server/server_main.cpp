#include "../../include/server/server.hpp"
#include "../../include/socket_guard.hpp"
#include "../../include/thread_pool.hpp"
#include <exception>
#include <iostream>

using namespace Thesisuis;

int main() {
    try {
        Server server;
        SocketGuard serverSocket(server.createSocket());
    
        server.bindSocket(serverSocket.get());
        server.listenForConnection(serverSocket.get());
        std::size_t num_workers = std::thread::hardware_concurrency();

        Pool pool(num_workers);
        std::cout << "Server started with " << num_workers << " workers" << std::endl;
        std::cout << "Waiting for connections..." << std::endl;
    
        while (true) {
            int client_fd = server.acceptConnection(serverSocket.get());
            std::cout << "Connection accepted from: " << client_fd << std::endl;
            pool.enqueue([client_fd, &server] {
                    server.handleClients(client_fd, server); 
                    });
        }
    } catch (const std::exception& e) {
        std::cout << "Fatal Error: " << e.what() << std::endl;
    }
    return 0;
}
