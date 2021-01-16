#include "Util.h"
#include <sstream>

std::vector<std::string> IconWrangler::split(const std::string& s, char splitChar) {
    std::vector<std::string> elements;

    std::stringstream wordStream;
    for (char readChar : s) {
        if (readChar == splitChar) {
            elements.push_back(wordStream.str());
            wordStream = std::stringstream{};
        } else {
            wordStream << readChar;
        }
    }
    elements.push_back(wordStream.str());

    return elements;
}