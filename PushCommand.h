#ifndef CS3258_CMAKE_PUSHCOMMAND_H
#define CS3258_CMAKE_PUSHCOMMAND_H

#include "Command.h"
#include "CLI.h"

class PushCommand : public Command {
public:
    PushCommand(CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~PushCommand();

private:
    std::string name;
    CLI& cli;
};


#endif //CS3258_CMAKE_PUSHCOMMAND_H
