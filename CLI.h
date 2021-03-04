#ifndef CS3258_CMAKE_CLI_H
#define CS3258_CMAKE_CLI_H

#include <string>

#include "Command.h"
#include "Tokenizer.h"
#include "TiffImageData.h"

class CLI {
public:
    // Constructor
    CLI(): hasImage(false), imageData(), totalReads(0), MAX_READS(1000) {}

    /*
     * method: isCommand
     * return: whether line begins with commen character or not
     */
    bool isCommand(std::string& line);

    /*
     * method: parseCommand
     * pre: line should not begin with a comment
     * return: nullptr if invalid command
     */
    Command* parseCommand(std::string& line, bool inFile);

    ~CLI();

    bool hasImage;
    TiffImageData imageData;
private:
    /*
     * method: tolower
     * return: token in lower case
     */
    void tolower(std::string& token);
    // used to parse input strings
    Tokenizer tokenizer;
    // path to append to filenames within directories
    std::string prefix;

    size_t totalReads;
    const size_t MAX_READS;
};


#endif //CS3258_CMAKE_CLI_H
