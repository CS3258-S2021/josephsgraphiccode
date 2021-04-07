#include "OrthoCommand.h"
#include "Matrix44.h"
#include <iostream>

OrthoCommand::OrthoCommand(double left, double right, double bottom, double top, double near, double far, CLI& mainCLI):
        name("ortho"), l(left), r(right), b(bottom), t(top), n(-near), f(-far), cli(mainCLI) {}

void OrthoCommand::execute() {
    if (r == l || b == t || n == f) {
        std::cout << "Clip window must have nonzero area, and f cannot equal n" << std::endl;
    } else {
        double r1[] = {2/(r-l), 0, 0, -(r+l)/(r-l)};
        double r2[] = {0, 2/(t-b), 0, -(t+b)/(t-b)};
        double r3[] = {0, 0, 2/(n-f), -(n+f)/(n-f)};
        double r4[] = {0, 0, 0, 1};
        Matrix44 orthoMatrix(r1, r2, r3, r4);
        cli.orthMatrix = orthoMatrix;
        cli.orthFlag = true;
        cli.perspFlag = false;
        cli.nearer = n;
        cli.farther = f;
    }
}

std::string OrthoCommand::toString() const {
    return name + ' ' + std::to_string(l) + ',' + std::to_string(r) + ',' + std::to_string(b) + ',' + std::to_string(t)
           + ',' + std::to_string(-n) + ',' + std::to_string(-f);
}