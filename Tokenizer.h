#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>

/*
 * Tokenizer splits string inputs into separate tokens
 */
class Tokenizer {
public:
    /*
     * method: tokenize
     * param: line - the string to tokenize
     * param: delims - string of individual delimiting characters
     * details: each token is separated by exactly 1 delimiting character. A single
     * trailing character is allowed at the end of the line
     * return: vector of tokens represented as strings
     */
    std::vector<std::string> tokenize(std::string& line, const std::string& delims);
};

#endif //TOKENIZER_H
