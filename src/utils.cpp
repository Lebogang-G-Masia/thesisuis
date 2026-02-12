#include "../include/utils.hpp"
#include <termios.h>
#include <unistd.h>
#include <iostream>

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

std::vector<std::string> split(std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}
