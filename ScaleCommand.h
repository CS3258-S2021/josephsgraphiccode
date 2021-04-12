#ifndef CS3258_CMAKE_SCALECOMMAND_H
#define CS3258_CMAKE_SCALECOMMAND_H

#include "Command.h"
#include "CLI.h"

class ScaleCommand : public Command {
public:
    ScaleCommand(double xIn, double yIn, double zIn, CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~ScaleCommand() {};

private:
    std::string name;
    double x, y, z;
    CLI& cli;
};


#endif //CS3258_CMAKE_SCALECOMMAND_H
