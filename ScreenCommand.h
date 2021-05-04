#ifndef CS3258_CMAKE_SCREENCOMMAND_H
#define CS3258_CMAKE_SCREENCOMMAND_H

#include "CLI.h"
#include "Command.h"

class ScreenCommand : public Command {
public:
    ScreenCommand(double width, double height, CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~ScreenCommand() {};
private:
    double width, height;
    std::string name;
    CLI& cli;
};


#endif //CS3258_CMAKE_SCREENCOMMAND_H
