#ifndef TIFFSTATCOMMAND_H
#define TIFFSTATCOMMAND_H

#include <string>

#include "Command.h"

class TiffStatCommand : public Command {
public:
    TiffStatCommand(std::string& file);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~TiffStatCommand() {};

private:
    std::string name;
    std::string fileName;
    std::string tagNames[33433];
};


#endif // TIFFSTATCOMMAND_H
