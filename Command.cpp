#include <iostream>

#include "Command.h"

void Command::execute() {
    std::cout << "Command execute" << std::endl;
}

std::string Command::toString() const {
    return "Command";
}
