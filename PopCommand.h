#ifndef CS3258_CMAKE_POPCOMMAND_H
#define CS3258_CMAKE_POPCOMMAND_H

#include "Command.h"
#include "CLI.h"

class PopCommand : public Command {
public:
    PopCommand(CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~PopCommand() {}

private:
    std::string name;
    CLI& cli;
};


#endif //CS3258_CMAKE_POPCOMMAND_H
