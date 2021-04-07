#include "RotateCommand.h"
#include "Matrix44.h"
#include "Vector3.h"

#include <cmath>

RotateCommand::RotateCommand(double tIn, double xIn, double yIn, double zIn, CLI& mainCLI):
        name("rotate"), theta(tIn), x(xIn), y(yIn), z(zIn), cli(mainCLI) {}

void RotateCommand::execute() {
    double angle = theta * M_PI / 180;
    Vector3 v(x, y, z);
    const Vector3& u(v.unit());
    double r1[] = {u[0]*u[0] + (1 - u[0]*u[0])*cos(angle),
                   u[0]*u[1]*(1 - cos(angle)) - u[2]*sin(angle),
                   u[0]*u[2]*(1 - cos(angle)) + u[1]*sin(angle),
                   0};

    double r2[] = {u[0]*u[1]*(1 - cos(angle)) + u[2]*sin(angle),
                   u[1]*u[1] + (1 - u[1]*u[1])*cos(angle),
                   u[1]*u[2]*(1 - cos(angle)) - u[0]*sin(angle),
                   0};

    double r3[] = {u[0]*u[2]*(1 - cos(angle)) - u[1]*sin(angle),
                   u[1]*u[2]*(1 - cos(angle)) + u[0]*sin(angle),
                   u[2]*u[2] + (1 - u[2]*u[2])*cos(angle),
                   0};

    double r4[] = {0, 0, 0, 1};

    Matrix44 rotation(r1, r2, r3, r4);
    cli.currentMatrix *= rotation;
}

std::string RotateCommand::toString() const {
    return name + ' ' + std::to_string(theta) + ',' + std::to_string(x) + ',' + std::to_string(y) + ',' + std::to_string(z);
}