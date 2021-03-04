#include "Tokenizer.h"
#include <iostream>

std::vector<std::string> Tokenizer::tokenize(std::string &line, const std::string &delims) {
    size_t start = 0;
    size_t end = 0;
    std::vector<std::string> tokens;
    while (start < line.length() && line[start] != '#') {
        bool matchDelims = false;
        while (end < line.length() && line[end] != '#' && !matchDelims) {
            for (const char& c : delims) {
                if (line[end] == c) {
                    matchDelims = true;
                }
            }
            ++end;
        }
        if (matchDelims) {
            tokens.push_back(line.substr(start, end-1-start));
        } else {    // Reached end of line
            tokens.push_back(line.substr(start, end-start));
        }
        start = end;
    }
    return tokens;
}