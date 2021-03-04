#ifndef ENDIANCHECKS_H
#define ENDIANCHECKS_H

#include <fstream>

    /*
     * checks what type of endian the architecture is
     */
    int is_Big_Endian();

    /*
     * swaps the endianness of the given input
     * @param var
     * @param numBytes (must be an even number)
     */
    void swapEndian(uint32_t *var, std::streamsize numBytes);

    /*
     * reads a file with endianness swapping taken into account
     * @param file
     * @Wparam var
     * @param numBytes
     * @param swap
     */
    void readEndian(std::ifstream &file, char *var, std::streamsize numBytes, bool swap);

#endif //ENDIANCHECKS_H
