#ifndef CS3258_CMAKE_ORTHOCOMMAND_H
#define CS3258_CMAKE_ORTHOCOMMAND_H

#include "Command.h"
#include "CLI.h"

class OrthoCommand : public Command {
public:
    OrthoCommand(double left, double right, double bottom, double top, double near, double far, CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~OrthoCommand() {}

private:
    std::string name;
    double l, r, b, t;
    double n, f;
    CLI& cli;
};


#endif //CS3258_CMAKE_ORTHOCOMMAND_H
