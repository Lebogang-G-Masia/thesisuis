#include "../include/utils.hpp"

void crash() {
    int* ptr = nullptr;
    *ptr = 1;
}
