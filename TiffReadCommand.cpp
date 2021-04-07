#include <fstream>
#include <iostream>
#include "TiffReadCommand.h"
#include "EndianChecks.h"
#include "Global.h"

extern "C" {
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
}

TiffReadCommand::TiffReadCommand(std::string& file, CLI& mainCLI): name("tiffread"), filename(file), cli(mainCLI) {}

void TiffReadCommand::execute() {
    std::cout << toString() << std::endl;

    std::ifstream imfile;
    imfile.open (filename, std::ios::binary);
    if (imfile.is_open()) {
        try {
            bool swap = false;
            bool isBigEndian = false;

            uint16_t endianness; // 2 byte quantity
            uint16_t magicNum; // 2 byte quantity
            uint32_t ifdaddress; // 4 byte quantity
            uint16_t ifdcount; // 2 byte quantity

            imfile.seekg (0, std::ios::beg); //not strictly necessary, but a good idea

            // match endianness
            imfile.read ((char *)&endianness, 2);
            if (endianness == 0x4d4d) {
                isBigEndian = true;
            } else if (endianness != 0x4949) {
                std::cout << "Error: Invalid TIFF format - cannot determine endianness" << std::endl;
                imfile.close();
                return;
            }
            if (isBigEndian != is_Big_Endian()) {
                swap = true;
            }

            readEndian(imfile, (char *)&magicNum, 2, swap);
            if (magicNum != 42) {
                std::cout << "Error: Not a TIFF file, magic number incorrect" << std::endl;
                imfile.close();
                return;
            }

            readEndian(imfile, (char *)&ifdaddress, 4, swap);
            // go to IFD position
            imfile.seekg(ifdaddress, std::ios::beg);
            readEndian(imfile, (char *)&ifdcount, 2, swap);

            for (uint16_t i = 0; i < ifdcount; i++) {
                uint16_t tag;
                uint16_t type;
                uint32_t count;
                readEndian(imfile, (char *)&tag, 2, swap);
                readEndian(imfile, (char *)&type, 2, swap);
                readEndian(imfile, (char *)&count, 4, swap);

                if (tag == 256) { // imageWidth
                    if (count == 1) {
                        if (type == 3) {
                            uint16_t value;
                            readEndian(imfile, (char *)&value, 2, swap);
                            cli.imageData.imageWidth = value;
                            if (value > checkImageWidth) {
                                std::cout << "Error: image width too large (max " << checkImageWidth << ")" << std::endl;
                                imfile.close();
                                return;
                            }
                            uint32_t junk;
                            readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                        } else if (type == 4) {
                            uint32_t value;
                            readEndian(imfile, (char *)&value, 4, swap);
                            cli.imageData.imageWidth = value;
                            if (value > checkImageWidth) {
                                std::cout << "Error: image width too large (max " << checkImageWidth << ")" << std::endl;
                                imfile.close();
                                return;
                            }
                        } else {
                            std::cout << "Error: invalid tag ImageWidth" << std::endl;
                            imfile.close();
                            return;
                        }
                    } else {
                        std::cout << "Error: invalid tag ImageWidth" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 257) { // imageLength
                    if (count == 1) {
                        if (type == 3) {
                            uint16_t value;
                            readEndian(imfile, (char *)&value, 2, swap);
                            cli.imageData.imageLength = value;
                            if (value > checkImageHeight) {
                                std::cout << "Error: image height too large (max " << checkImageHeight << ")" << std::endl;
                                imfile.close();
                                return;
                            }
                            uint32_t junk;
                            readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                        } else if (type == 4) {
                            uint32_t value;
                            readEndian(imfile, (char *)&value, 4, swap);
                            cli.imageData.imageLength = value;
                            if (value > checkImageHeight) {
                                std::cout << "Error: image height too large (max " << checkImageHeight << ")" << std::endl;
                                imfile.close();
                                return;
                            }
                        } else {
                            std::cout << "Error: invalid tag ImageLength" << std::endl;
                            imfile.close();
                            return;
                        }
                    } else {
                        std::cout << "Error: invalid tag ImageLength" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 258) {    // bitsPerSample
                    if (type == 3) {
                        if (count == 1) {
                            uint16_t value;
                            readEndian(imfile, (char *)&value, 2, swap);
                            cli.imageData.bitsPerSample[0] = value;
                            if (value != 8) {
                                std::cout << "Error: grayscale image must have 8-bit sampling" << std::endl;
                                imfile.close();
                                return;
                            }
                            uint32_t junk;
                            readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                        } else if (count == 3) {
                            uint32_t offset;
                            readEndian(imfile, (char *)&offset, 4, swap);
                            std::streampos prevPos = imfile.tellg();
                            imfile.seekg(offset, std::ios::beg);
                            uint16_t value;
                            for (uint32_t j = 0; j < count; j++) {
                                readEndian(imfile, (char *)&value, 2, swap);
                                cli.imageData.bitsPerSample[j] = value;
                                if (value != 8) {
                                    std::cout << "Error: RGB image must have 8/8/8 bit sampling" << std::endl;
                                    imfile.close();
                                    return;
                                }
                            }
                            imfile.seekg(prevPos, std::ios::beg);
                        } else {
                            std::cout << "Error: tag bitsPerSample does not fit grayscale or RGB image" << std::endl;
                            imfile.close();
                            return;
                        }
                    } else {
                        std::cout << "Error: invalid tag bitsPerSample" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 259) {    // compression
                    if (count == 1 && type == 3) {
                        uint16_t value;
                        readEndian(imfile, (char *)&value, 2, swap);
                        cli.imageData.compression = value;
                        if (value != 1) {
                            std::cout << "Error: cannot read compressed image" << std::endl;
                            imfile.close();
                            return;
                        }
                        uint32_t junk;
                        readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                    } else {
                        std::cout << "Error: invalid tag Compression" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 262) {    // photometric
                    if (count == 1 && type == 3) {
                        uint16_t value;
                        readEndian(imfile, (char *)&value, 2, swap);
                        uint32_t junk;
                        readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                        cli.imageData.photometric = value;
                        if (value > 2) {
                            std::cout << "Error: image is not grayscale or RGB" << std::endl;
                            imfile.close();
                            return;
                        } else {
                            cli.imageData.isGrayscale = value < 2;
                        }
                    } else {
                        std::cout << "Error: invalid tag PhotometricInterpretation" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 273) {    // stripOffsets
                    if (type == 3) {
                        if (count < 3) {
                            uint16_t value;
                            uint32_t *values = new uint32_t[count];
                            for (uint32_t j = 0; j < count; j++) {
                                readEndian(imfile, (char *)&value, 2, swap);
                                values[j] = value;
                            }
                            cli.imageData.setStripOffsets(values);
                            uint32_t junk;
                            readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                        } else {
                            uint32_t offset;
                            readEndian(imfile, (char *)&offset, 4, swap);
                            std::streampos prevPos = imfile.tellg();
                            imfile.seekg(offset, std::ios::beg);
                            uint16_t value;
                            uint32_t *values = new uint32_t[count];
                            for (uint32_t j = 0; j < count; j++) {
                                readEndian(imfile, (char *)&value, 2, swap);
                                values[j] = value;
                            }
                            cli.imageData.setStripOffsets(values);
                            imfile.seekg(prevPos, std::ios::beg);
                        }
                        cli.imageData.numStrips = count;
                    } else if (type == 4) {
                        std::streampos prevPos;
                        if (count > 1) {
                            uint32_t offset;
                            readEndian(imfile, (char *)&offset, 4, swap);
                            prevPos = imfile.tellg();
                            imfile.seekg(offset, std::ios::beg);
                        }
                        uint32_t value;
                        uint32_t *values = new uint32_t[count];
                        for (uint32_t j = 0; j < count; j++) {
                            readEndian(imfile, (char *)&value, 4, swap);
                            values[j] = value;
                        }
                        cli.imageData.setStripOffsets(values);
                        cli.imageData.numStrips = count;
                        if (count > 1) {
                            imfile.seekg(prevPos, std::ios::beg);
                        }
                    } else {
                        std::cout << "Error: invalid tag stripOffsets" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 277) {    // samplesPerPixel
                    if (count == 1 && type == 3) {
                        uint16_t value;
                        readEndian(imfile, (char *)&value, 2, swap);
                        cli.imageData.samplesPerPixel = value;
                        if (value != 3 && value != 1) {
                            std::cout << "Error: incorrect number of sample in RGB image" << std::endl;
                            imfile.close();
                            return;
                        }
                        uint32_t junk;
                        readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                    } else {
                        std::cout << "Error: invalid tag SamplesPerPixel" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 278) {    // rowsPerStrip
                    if (count == 1) {
                        if (type == 3) {
                            uint16_t value;
                            readEndian(imfile, (char *)&value, 2, swap);
                            cli.imageData.rowsPerStrip = value;
                            uint32_t junk;
                            readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                        } else if (type == 4) {
                            uint32_t value;
                            readEndian(imfile, (char *)&value, 4, swap);
                            cli.imageData.rowsPerStrip = value;
                        } else {
                            std::cout << "Error: invalid tag RowsPerStrip" << std::endl;
                            imfile.close();
                            return;
                        }
                    } else {
                        std::cout << "Error: invalid tag RowsPerStrip" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 279) {    // stripByteCounts
                    if (type == 3) {
                        if (count < 3) {
                            uint16_t value;
                            uint32_t *values = new uint32_t[count];
                            for (uint32_t j = 0; j < count; j++) {
                                readEndian(imfile, (char *)&value, 2, swap);
                                values[j] = value;
                            }
                            cli.imageData.setStripByteCounts(values);
                            uint32_t junk;
                            readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                        } else {
                            uint32_t offset;
                            readEndian(imfile, (char *)&offset, 4, swap);
                            std::streampos prevPos = imfile.tellg();
                            imfile.seekg(offset, std::ios::beg);
                            uint16_t value;
                            uint32_t *values = new uint32_t[count];
                            for (uint32_t j = 0; j < count; j++) {
                                readEndian(imfile, (char *)&value, 2, swap);
                                values[j] = value;
                            }
                            cli.imageData.setStripByteCounts(values);
                            imfile.seekg(prevPos, std::ios::beg);
                        }
                    } else if (type == 4) {
                        std::streampos prevPos;
                        if (count > 1) {
                            uint32_t offset;
                            readEndian(imfile, (char *)&offset, 4, swap);
                            prevPos = imfile.tellg();
                            imfile.seekg(offset, std::ios::beg);
                        }
                        uint32_t value;
                        uint32_t *values = new uint32_t[count];
                        for (uint32_t j = 0; j < count; j++) {
                            readEndian(imfile, (char *)&value, 4, swap);
                            values[j] = value;
                        }
                        cli.imageData.setStripByteCounts(values);
                        if (count > 1) {
                            imfile.seekg(prevPos, std::ios::beg);
                        }
                    } else {
                        std::cout << "Error: invalid tag StripByteCounts" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 282) {    // XResolution
                    if (count == 1 && type == 5) {
                        uint32_t offset;
                        readEndian(imfile, (char *)&offset, 4, swap);
                        std::streampos prevPos = imfile.tellg();
                        imfile.seekg(offset, std::ios::beg);

                        uint32_t numer;
                        uint32_t denom;
                        readEndian(imfile, (char *)&numer, 4, swap);
                        readEndian(imfile, (char *)&denom, 4, swap);
                        cli.imageData.xResNum = numer;
                        cli.imageData.xResDen = denom;

                        imfile.seekg(prevPos, std::ios::beg);
                    } else {
                        std::cout << "Error: invalid tag XResolution" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 283) {    // YResolution
                    if (count == 1 && type == 5) {
                        uint32_t offset;
                        readEndian(imfile, (char *)&offset, 4, swap);
                        std::streampos prevPos = imfile.tellg();
                        imfile.seekg(offset, std::ios::beg);

                        uint32_t numer;
                        uint32_t denom;
                        readEndian(imfile, (char *)&numer, 4, swap);
                        readEndian(imfile, (char *)&denom, 4, swap);
                        cli.imageData.yResNum = numer;
                        cli.imageData.yResDen = denom;

                        imfile.seekg(prevPos, std::ios::beg);
                    } else {
                        std::cout << "Error: invalid tag YResolution" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 284) {    // PlanarConfiguration
                    if (count == 1 && type == 3) {
                        uint16_t value;
                        readEndian(imfile, (char *)&value, 2, swap);
                        if (value != 1) {
                            std::cout << "Error: cannot handle non-chunky format" << std::endl;
                            imfile.close();
                            return;
                        }
                        uint32_t junk;
                        readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                    } else {
                        std::cout << "Error: invalid tag PlanarConfiguration" << std::endl;
                        imfile.close();
                        return;
                    }
                } else if (tag == 296) {    // ResolutionUnit
                    if (count == 1 && type == 3) {
                        uint16_t value;
                        readEndian(imfile, (char *)&value, 2, swap);
                        cli.imageData.resolutionUnit = value;
                        if (value > 3 || value == 0) {
                            std::cout << "Error: invalid tag ResolutionUnit" << std::endl;
                            imfile.close();
                            return;
                        }
                        uint32_t junk;
                        readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                    } else {
                        std::cout << "Error: invalid tag ResolutionUnit" << std::endl;
                        imfile.close();
                        return;
                    }
                } else {
                    imfile.seekg(imfile.tellg() + (std::streampos)4, std::ios::beg);
                }
            }

            // clear image buffer
            makeCheckImage();

            // Read every strip but the last one
            for (uint32_t i = 0; i < cli.imageData.numStrips - 1; i++) {
                uint32_t offset = cli.imageData.stripOffsets[i];
                imfile.seekg(offset, std::ios::beg);
                uint32_t startRow = i * cli.imageData.rowsPerStrip;
                for (uint32_t row = startRow; row < startRow + cli.imageData.rowsPerStrip; row++) {
                    for (uint32_t col = 0; col < cli.imageData.imageWidth; col++) {
                        if (cli.imageData.isGrayscale) {
                            uint8_t value;
                            readEndian(imfile, (char *)&value, 1, swap);
                            for (uint8_t depth = 0; depth < 3; depth++) {
                                checkImage[cli.imageData.imageLength-row-1][col][depth] = (GLubyte) value;
                            }
                        } else {
                            uint8_t red, green, blue;
                            readEndian(imfile, (char *)&red, 1, swap);
                            readEndian(imfile, (char *)&green, 1, swap);
                            readEndian(imfile, (char *)&blue, 1, swap);
                            checkImage[cli.imageData.imageLength-row-1][col][0] = (GLubyte) red;
                            checkImage[cli.imageData.imageLength-row-1][col][1] = (GLubyte) green;
                            checkImage[cli.imageData.imageLength-row-1][col][2] = (GLubyte) blue;
                        }
                    }
                }
            }

            //Read the last strip
            uint32_t offset = cli.imageData.stripOffsets[cli.imageData.numStrips-1];
            imfile.seekg(offset, std::ios::beg);
            uint32_t startRow = (cli.imageData.numStrips - 1) * cli.imageData.rowsPerStrip;
            for (uint32_t row = startRow; row < startRow + cli.imageData.imageLength - cli.imageData.rowsPerStrip * (cli.imageData.numStrips - 1); row++) {
                for (uint32_t col = 0; col < cli.imageData.imageWidth; col++) {
                    if (cli.imageData.isGrayscale) {
                        uint8_t value;
                        readEndian(imfile, (char *)&value, 1, swap);
                        if (cli.imageData.photometric == 0) {
                            for (uint8_t depth = 0; depth < 3; depth++) {
                                checkImage[cli.imageData.imageLength-row-1][col][depth] = (GLubyte)(255 - value);
                            }
                        } else {
                            for (uint8_t depth = 0; depth < 3; depth++) {
                                checkImage[cli.imageData.imageLength-row-1][col][depth] = (GLubyte) value;
                            }
                        }
                    } else {
                        uint8_t red, green, blue;
                        readEndian(imfile, (char *)&red, 1, swap);
                        readEndian(imfile, (char *)&green, 1, swap);
                        readEndian(imfile, (char *)&blue, 1, swap);
                        checkImage[cli.imageData.imageLength-row-1][col][0] = (GLubyte) red;
                        checkImage[cli.imageData.imageLength-row-1][col][1] = (GLubyte) green;
                        checkImage[cli.imageData.imageLength-row-1][col][2] = (GLubyte) blue;
                    }
                }
            }

            //Image should now be successfully read
            cli.hasImage = true;

            glutPostRedisplay();
        } catch (std::exception& ex) {
            imfile.close();
            std::cout << "Error: unable to read file as TIFF format" << std::endl;
        }
    } else {
        std::cout << "Error: invalid file name \"" << filename << "\"" << std::endl;
    }
}
std::string TiffReadCommand::toString() const {
    return name + " \"" + filename + "\"";
}