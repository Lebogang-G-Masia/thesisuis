#include "../include/utils.hpp"
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>


void crash() {
    int* ptr = nullptr;
    *ptr = 1;
}

std::string userInput(bool hide) {
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

std::vector<std::string_view> split(std::string_view& s, const std::string_view& delimiter) {
    std::vector<std::string_view> tokens;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string_view::npos) {
        tokens.push_back(s.substr(0, pos));
        s.remove_prefix(pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

void sendData(int clientSocket, std::string& msg) {
    uint32_t length = htonl(msg.length());
    send(clientSocket, &length, sizeof(length), 0);
    send(clientSocket, msg.c_str(), msg.length(), 0);
}

std::string receiveData(int clientSocket) {
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
