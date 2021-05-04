//
// Created by chang on 5/2/2021.
//

#ifndef CS3258_CMAKE_TRACECOMMAND_H
#define CS3258_CMAKE_TRACECOMMAND_H

#include "CLI.h"
#include "Vector3.h"
#include "Ray.h"

class TraceCommand : public Command {
public:
    TraceCommand(CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~TraceCommand() {};
private:
    Vector3 reflection(Ray direction, int depth);
    std::string name;
    CLI& cli;
    double eps;
};


#endif //CS3258_CMAKE_TRACECOMMAND_H
