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

