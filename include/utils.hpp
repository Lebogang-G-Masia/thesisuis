#include <string>
#include <vector>

#ifndef THESISUIS_UTILS
#define THESISUIS_UTILS

void crash();
// get user input
std::string userInput(bool = false);
// Split string using delimeter
std::vector<std::string> split(std::string&, const std::string& =":");

#endif // THESISUIS_UTILS
