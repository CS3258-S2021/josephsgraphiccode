//
// Created by chang on 5/2/2021.
//

#ifndef CS3258_CMAKE_CLEARCOMMAND_H
#define CS3258_CMAKE_CLEARCOMMAND_H

#include "CLI.h"

class ClearCommand : public Command {
public:
    ClearCommand(CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~ClearCommand() {};
private:
    std::string name;
    CLI& cli;
};


#endif //CS3258_CMAKE_CLEARCOMMAND_H
