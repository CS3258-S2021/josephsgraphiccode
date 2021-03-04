#include <algorithm>

#include "EndianChecks.h"

int is_Big_Endian() {
    union {
        uint32_t i;
        char c[4];
    } x = {0x01020304};
    return x.c[0] == 1;
}

void readEndian(std::ifstream &file, char *var, std::streamsize numBytes, bool swap) {
    file.read(var, numBytes);
    if (swap) {
        swapEndian((uint32_t *)var, numBytes);
    }
}

void swapEndian(uint32_t *var, std::streamsize numBytes) {
    auto *tmp = (unsigned char *)var;
    for (std::streamsize i = 0; i < numBytes/2; i++) {
        std::swap(tmp[i], tmp[numBytes - 1 - i]);
    }
}
