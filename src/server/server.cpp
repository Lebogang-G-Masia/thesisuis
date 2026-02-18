#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <stdexcept>
#include <stdlib.h>
#include <algorithm>
#include "../../include/server/server.hpp"
#include "../../include/utils.hpp"
#include "../../include/socket_guard.hpp"

using namespace Thesisuis;

Server::Server() {
    // Initialize the server
    // default credentials:
    // username = root
    // password = toor
    std::fstream creds;
    creds.open("creds.txt");
    if (!creds.is_open()) {
        std::string username = "root";
        std::string password = "toor";
        std::string hash = hash_password(password);
        addUser(username, hash);
    }
    creds.close();
    // Add commands to the vector
    commands.push_back("add_user");
}

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
            if (username != uname) {
                continue;
            }
            std::string_view password_hash = split_creds.at(1);
            if (password_hash == hash) return true;

        }
    } 
    return false;
}

void Server::addUser(std::string uname, std::string passwd, bool exists) {
    if (!exists) {
        std::ofstream creds("creds.txt");
        if (creds.is_open()) {
            creds << uname << ":" << passwd << std::endl;
        }
        creds.close();
    } else {
        std::ofstream creds;
        creds.open("creds.txt", std::ios::app);
        if (creds.is_open()) {
            creds << uname << ":" << passwd << std::endl;
        }
        creds.close();
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

void Server::handleClients(int rawSocket, Server& server) {
    SocketGuard clientSocket(rawSocket);
    try {
        if (!server.authenticate(clientSocket.get())) {
            std::cerr << "Authentication failed for the socket: " << clientSocket.get() << std::endl;
            return;
        }
        //std::cout << "Client authenticated on thread: " << std::this_thread::get_id();
        std::string cmd = receiveData(clientSocket.get());
        while (!cmd.empty() && cmd != "exit") {
            std::cout << "[Client " << clientSocket.get() << "]: " << cmd << std::endl;
            if (std::find(commands.begin(), commands.end(), cmd) != commands.end()) {
                if (cmd == "add_user") {
                    std::string username = receiveData(clientSocket.get());
                    std::string password = receiveData(clientSocket.get());
                    std::string hash = hash_password(password);
                    addUser(username, hash, true);
                    std::string response = "User created";
                    sendData(clientSocket.get(), response);
                }
            } else {
                std::string error = "Error: Command " + cmd + " does not exist";
                sendData(clientSocket.get(), error);
            }
            cmd = receiveData(clientSocket.get());
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Closing connection for socket: " << clientSocket.get() << std::endl;
}

