//
// Created by chang on 5/2/2021.
//

#include "ClearCommand.h"
#include "Global.h"
#include <iostream>

ClearCommand::ClearCommand(CLI &mainCLI) : name("clear"), cli(mainCLI) {}

void ClearCommand::execute() {
    makeCheckImage();
    for (auto shape : cli.shapes) {
        delete shape;
    }
    cli.shapes.clear();
    cli.lights.clear();
    cli.hasImage = false;
    glutPostRedisplay();
}

std::string ClearCommand::toString() const {
    return name;
}