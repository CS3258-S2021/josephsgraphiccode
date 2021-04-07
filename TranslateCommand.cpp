#include "TranslateCommand.h"
#include "Matrix44.h"

TranslateCommand::TranslateCommand(double xIn, double yIn, double zIn, CLI &mainCLI):
    name("translate"), x(xIn), y(yIn), z(zIn), cli(mainCLI) {}

void TranslateCommand::execute() {
    Matrix44 translation;
    translation.set(0, 3, x);
    translation.set(1, 3, y);
    translation.set(2, 3, z);
    cli.currentMatrix *= translation;
}

std::string TranslateCommand::toString() const {
    return name + ' ' + std::to_string(x) + ',' + std::to_string(y) + ',' + std::to_string(z);
}


