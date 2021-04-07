#ifndef CS3258_CMAKE_VERTEXCOMMAND_H
#define CS3258_CMAKE_VERTEXCOMMAND_H

#include "Command.h"
#include "CLI.h"
#include "Vector4.h"

class VertexCommand : public Command {
public:
    VertexCommand(float xIn, float yIn, float zIn, CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~VertexCommand() {}

private:
    std::string name;
    float x, y, z;

    CLI& cli;
};


#endif //CS3258_CMAKE_VERTEXCOMMAND_H
