//
// Created by chang on 5/2/2021.
//

#include "OrthocameraCommand.h"

Ray OrthocameraCommand::getRay(double x, double y) {
    return Ray(Vector3(x, y, 0), Vector3(0, 0, -1));
}

OrthocameraCommand::~OrthocameraCommand() {}