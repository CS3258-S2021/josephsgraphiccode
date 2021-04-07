#include "PushCommand.h"
#include <iostream>

PushCommand::PushCommand(CLI &mainCLI): name("push"), cli(mainCLI) {}

void PushCommand::execute() {
    cli.stack.push_back(cli.currentMatrix);
}

std::string PushCommand::toString() const {
    return name;
}