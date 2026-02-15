#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <fstream>
#include <stdexcept>
#include "../../include/server/server.hpp"
#include "../../include/utils.hpp"
#include "../../include/socket_guard.hpp"

using namespace Thesisuis;

std::string Server::hash_password(const std::string& password) {
    std::hash<std::string> hasher;
    std::size_t hashed = hasher(password);

    std::stringstream ss;
    ss << std::hex << hashed;
    return ss.str();
}

bool Server::authenticate(int clientSocket) {
    std::string uname = receiveData(clientSocket);
    std::string passwd = receiveData(clientSocket);

    // hash the password
    std::string hash = hash_password(passwd);

    // try opening credentials file for reading
    std::ifstream credsFile("creds.txt");
    if (credsFile.is_open()) {
        std::string cred {};
        while(std::getline(credsFile, cred)) {
            std::string_view cred_v = cred;
            std::vector<std::string_view> split_creds = split(cred_v);
            if (split_creds.size() < 2) continue;

            std::string_view username = split_creds.at(0);
            if (username != uname) continue;
            std::string_view password_hash = split_creds.at(1);
            if (password_hash == hash) return true;

        }
    } else {
        std::cout << "Creating the user" << std::endl;
        addUser(uname, passwd);
        return true;
    }
    return false;
}

void Server::addUser(std::string uname, std::string passwd, bool exists) {
    if (exists) {
        std::ofstream creds;
        creds.open("creds.txt", std::ios::app);

        if (creds.is_open()) {
            creds << uname << ":" << passwd << std::endl;
            creds.close();
        }
    } else {
        std::ofstream creds("creds.txt");
        if (creds.is_open()) {
            creds << uname << ":" << passwd << std::endl;
            creds.close();
        }
    }
}

int Server::createSocket() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        throw std::runtime_error("Failed to create the server socket");
    }
    return serverSocket;
}

sockaddr_in Server::bindSocket(int serverSocket) {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        throw std::runtime_error("Failed to set socket option");
    }

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        throw std::runtime_error("Failed to bind the socket");
    }
    return serverAddress;
}

void Server::listenForConnection(int serverSocket) {
    if (listen(serverSocket, 5) == -1) {
        throw std::runtime_error("Failed to listen");
    }
    std::cout << "Listening on port 8080..." << std::endl;
}

int Server::acceptConnection(int serverSocket) {
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        throw std::runtime_error("Failed to accept a connection");
    }
    return clientSocket;
}

std::string Server::receiveData(int clientSocket) {
    uint32_t network_length = 0;
    int bytesRecv = recv(clientSocket, &network_length, sizeof(network_length), 0);
    if (bytesRecv <= 0) return "";
    // convert back to host bytes order
    uint32_t length = ntohl(network_length);

    std::vector<char> buffer(length);
    uint32_t totalRecv = 0;

    while (totalRecv < length) {
        int b = recv(clientSocket, buffer.data() + totalRecv, length - totalRecv, 0);
        if (b <= 0) return "";
        totalRecv += b;
    }
    return std::string(buffer.data(), totalRecv);
}

void Server::closeSockets(int serverSocket, int clientSocket) {
    close(serverSocket);
    close(clientSocket);
}

void Server::handleClients(int rawSocket, Server& server) {
    SocketGuard clientSocket(rawSocket);
    try {
        if (!server.authenticate(clientSocket.get())) {
            std::cerr << "Authentication failed for the socket: " << clientSocket.get() << std::endl;
            return;
        }
        std::cout << "Client authenticated on thread: " << std::this_thread::get_id();
        std::string msg = server.receiveData(clientSocket.get());
        while (!msg.empty() && msg != "exit") {
            std::cout << "[Client " << clientSocket.get() << "]: " << msg << std::endl;
            msg = server.receiveData(clientSocket.get());
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Closing connection for socket: " << clientSocket.get() << std::endl;
}
