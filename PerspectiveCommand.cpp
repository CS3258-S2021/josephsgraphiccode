#include "PerspectiveCommand.h"
#include "Matrix44.h"

#include <iostream>
#include <cmath>

PerspectiveCommand::PerspectiveCommand(double angle, double r, double near, double far, CLI& mainCLI):
        name("perspective"), theta(angle), ratio(r), n(-near), f(-far), cli(mainCLI) {}

void PerspectiveCommand::execute() {
    if (theta == 0 || theta >= 180 || ratio == 0 || n == f) {
        std::cout << "Clip window must have nonzero area, and f cannot equal n, and the angle must be less than 180 degrees" << std::endl;
    } else {
        double angle = theta * M_PI / 180;
        double r1[] = {(1/ratio)*(1/tan(angle/2)), 0, 0, 0};
        double r2[] = {0, 1/tan(angle/2), 0, 0};
        double r3[] = {0, 0, f/(f-n), -f*n/(f-n)};
        double r4[] = {0, 0, 1, 0};
        Matrix44 perspectiveMatrix(r1, r2, r3, r4);
        cli.perspMatrix = perspectiveMatrix;
        cli.orthFlag = false;
        cli.perspFlag = true;
        cli.near = n;
        cli.far = f;
    }
}

std::string PerspectiveCommand::toString() const {
    return name + ' ' + std::to_string(theta) + ',' + std::to_string(ratio)
           + ',' + std::to_string(-n) + ',' + std::to_string(-f);
}