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
