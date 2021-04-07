#include "ScaleCommand.h"
#include "Matrix44.h"

ScaleCommand::ScaleCommand(double xIn, double yIn, double zIn, CLI& mainCLI):
        name("scale"), x(xIn), y(yIn), z(zIn), cli(mainCLI) {}

void ScaleCommand::execute() {
    Matrix44 scale;
    scale.set(0, 0, x);
    scale.set(1, 1, y);
    scale.set(2, 2, z);
    cli.currentMatrix *= scale;
}

std::string ScaleCommand::toString() const {
    return name + ' ' + std::to_string(x) + ',' + std::to_string(y) + ',' + std::to_string(z);
}
