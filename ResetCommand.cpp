#include "ResetCommand.h"
#include "Global.h"
#include <iostream>

ResetCommand::ResetCommand(CLI& mainCLI): name("reset"), cli(mainCLI) {}

void ResetCommand::execute() {
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0,0);
    glFlush();

    cli.stack.clear();
    cli.currentMatrix = Matrix44();

    makeBlackImage();
}

std::string ResetCommand::toString() const {
    return name;
}