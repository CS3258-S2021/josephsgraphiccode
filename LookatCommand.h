#ifndef CS3258_CMAKE_LOOKATCOMMAND_H
#define CS3258_CMAKE_LOOKATCOMMAND_H

#include "Command.h"
#include "CLI.h"

class LookatCommand : public Command {
public:
    LookatCommand(float fX, float fY, float fZ, float atX, float atY, float atZ, float upX, float upY, float upZ, CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~LookatCommand() {}

private:
    std::string name;
    float fx, fy, fz;
    float atx, aty, atz;
    float upx, upy, upz;
    CLI& cli;
};


#endif //CS3258_CMAKE_LOOKATCOMMAND_H
