#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include "../../include/server/server.hpp"
#include "../../include/utils.hpp"

using namespace Thesisuis;

bool Server::authenticate(int clientSocket) {
    std::string uname = receiveData(clientSocket);
    std::string passwd = receiveData(clientSocket);
    // try opening credentials file for reading
    std::ifstream credsFile("creds.txt");
    if (credsFile.is_open()) {
        std::string cred {};
        while(std::getline(credsFile, cred)) {
            std::vector<std::string> split_creds = split(cred);
            std::string username = split_creds.at(0);
            if (username != uname) continue;
            std::string password = split_creds.at(1);
            if (password == passwd) return true;

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
        std::cerr << "Failed to create the server socket" << std::endl;
        exit(1);
    }
    return serverSocket;
}

sockaddr_in Server::bindSocket(int serverSocket) {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind the socket" << std::endl;
        exit(1);
    }
    return serverAddress;
}

void Server::listenForConnection(int serverSocket) {
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Failed to listen for a connection" << std::endl;
        exit(1);
    }
    std::cout << "Listening on port 8080..." << std::endl;
}

int Server::acceptConnection(int serverSocket) {
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept connection" << std::endl;
        exit(1);
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

