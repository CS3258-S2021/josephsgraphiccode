#ifndef CS3258_CMAKE_RESETCOMMAND_H
#define CS3258_CMAKE_RESETCOMMAND_H

#include "Command.h"
#include "CLI.h"

class ResetCommand : public Command {
public:
    ResetCommand(CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~ResetCommand() {}

private:
    std::string name;
    CLI& cli;
};


#endif //CS3258_CMAKE_RESETCOMMAND_H
