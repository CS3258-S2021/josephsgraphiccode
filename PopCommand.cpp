#include "PopCommand.h"
#include <iostream>

PopCommand::PopCommand(CLI &mainCLI): name("pop"), cli(mainCLI) {}

void PopCommand::execute() {
    if (cli.stack.empty()) {
        std::cout << "Error: no matrices to pop" << std::endl;
    } else {
        cli.currentMatrix = cli.stack.back();
        cli.stack.pop_back();
    }
}

std::string PopCommand::toString() const {
    return name;
}