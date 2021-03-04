#ifndef TIFFREADCOMMAND_H
#define TIFFREADCOMMAND_H

#include <string>
#include "Command.h"
#include "CLI.h"

class TiffReadCommand : public Command {
public:
    TiffReadCommand(std::string& file, CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~TiffReadCommand() {};
private:
    std::string name;
    std::string filename;
    CLI& cli;
};


#endif //TIFFREADCOMMAND_H
