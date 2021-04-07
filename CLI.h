#ifndef CS3258_CMAKE_CLI_H
#define CS3258_CMAKE_CLI_H

#include <string>

#include "Command.h"
#include "Tokenizer.h"
#include "TiffImageData.h"
#include "Filter.h"
#include "Matrix44.h"
#include "Vector4.h"

class CLI {
public:
    // Constructor
    CLI(): hasImage(false), imageData(), filter(), perspFlag(false), orthFlag(false),
            nearer(0), farther(0), perspMatrix(), orthMatrix(), currentMatrix(), stack(),
            printFlag(0), savemat(), totalReads(0), MAX_READS(1000) {}

    /*
     * method: isCommand
     * return: whether line begins with common character or not
     */
    static bool isCommand(std::string& line);

    /*
     * method: parseCommand
     * pre: line should not begin with a comment
     * return: nullptr if invalid command
     */
    Command* parseCommand(std::string& line, bool inFile);

    ~CLI();

    bool hasImage;
    TiffImageData imageData;
    Filter filter;

    bool perspFlag, orthFlag;
    double nearer, farther;
    Matrix44 perspMatrix, orthMatrix;
    Matrix44 currentMatrix;
    std::vector<Matrix44> stack;
    int printFlag;
    Vector4 savemat;
private:
    /*
     * method: tolower
     * return: token in lower case
     */
    static void tolower(std::string& token);
    // used to parse input strings
    Tokenizer tokenizer;
    // path to append to filenames within directories
    std::string prefix;

    size_t totalReads;
    const size_t MAX_READS;
};


#endif //CS3258_CMAKE_CLI_H
