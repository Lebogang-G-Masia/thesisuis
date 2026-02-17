#include <string_view>
#include <vector>

#ifndef THESISUIS_UTILS
#define THESISUIS_UTILS

void crash();
// get user input
std::string userInput(bool = false);
// Split string using delimeter
std::vector<std::string_view> split(std::string_view&, const std::string_view& =":");
void sendData(int, std::string& msg);
std::string receiveData(int);

#endif // THESISUIS_UTILS
