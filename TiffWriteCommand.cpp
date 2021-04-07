#include <iostream>
#include "TiffWriteCommand.h"
#include "EndianChecks.h"
#include "Global.h"

TiffWriteCommand::TiffWriteCommand(std::string& file, std::vector<double>& p, CLI& mainCLI): name("tiffwrite"), fileName(file), params(p), cli(mainCLI) {}

void TiffWriteCommand::execute() {
    std::cout << toString() << std::endl;

    if (!cli.hasImage) {
        std::cout << "Error: please read an image first" << std::endl;
        return;
    }

    uint32_t x0 = (uint32_t)((int)params[0]);
    uint32_t y0 = (uint32_t)((int)params[1]);
    uint32_t xc = (uint32_t)((int)params[2]);
    uint32_t yc = (uint32_t)((int)params[3]);
    if (x0 < 0 || y0 < 0 || xc > cli.imageData.imageWidth || yc > cli.imageData.imageLength) {
        std::cout << "Error: parameters out of bounds" << std::endl;
        return;
    }

    uint32_t imageHeight = yc - y0;
    uint32_t imageWidth = xc - x0;

    std::ofstream imfile(fileName, std::ios::binary | std::ios::out);
    if (imfile.is_open()) {
        try {
            uint16_t endianness = is_Big_Endian() ? (uint16_t)0x4d4d : (uint16_t)0x4949;
            uint16_t magic = 42;
            uint32_t ifdaddress = 8;

            imfile.seekp(0, std::ios::beg);
            imfile.write((char *)&endianness, 2);
            imfile.write((char *)&magic, 2);
            imfile.write((char *)&ifdaddress, 4);

            std::streampos afterifd = imfile.tellp() + (std::streampos)(2 + 144 + 4);

            uint16_t numEntries = 12;
            imfile.write((char *)&numEntries, 2);

            uint16_t tag;
            uint16_t type;
            uint32_t count;
            uint32_t offset;

            // ImageWidth
            tag = 256;
            type = 4;
            count = 1;
            offset = imageWidth;
            writeIFDEntry(imfile, tag, type, count, offset);

            // ImageHeight
            tag = 257;
            type = 4;
            count = 1;
            offset = imageHeight;
            writeIFDEntry(imfile, tag, type, count, offset);

            // BitsPerSample
            tag = 258;
            type = 3;
            if (cli.imageData.isGrayscale) {
                count = 1;
                offset = cli.imageData.bitsPerSample[0];
                writeIFDEntry(imfile, tag, type, count, offset);
            } else {
                count = 3;
                offset = (uint32_t)afterifd;
                std::streampos prevPos = imfile.tellp();
                imfile.seekp(afterifd, std::ios::beg);
                for (uint32_t i = 0; i < count; i++) {
                    uint16_t value = cli.imageData.bitsPerSample[i];
                    imfile.write((char *)&value, 2);
                }
                afterifd = imfile.tellp();
                imfile.seekp(prevPos, std::ios::beg);
                writeIFDEntry(imfile, tag, type, count, offset);
            }

            // Compression
            tag = 259;
            type = 3;
            count = 1;
            offset = cli.imageData.compression;
            writeIFDEntry(imfile, tag, type, count, offset);

            // PhotometricInterpretation
            tag = 262;
            type = 3;
            count = 1;
            offset = cli.imageData.photometric;
            writeIFDEntry(imfile, tag, type, count, offset);

            // StripOffsets
            tag = 273;
            type = 4;
            count = 1;
            offset = (uint32_t)afterifd;
            writeIFDEntry(imfile, tag, type, count, offset);

            // Write Image Data
            std::streampos prevPos = imfile.tellp();
            imfile.seekp(afterifd, std::ios::beg);
            for (uint32_t row = y0; row < yc; row++) {
                for (uint32_t col = x0; col < xc; col++) {
                    if (cli.imageData.isGrayscale) {
                        uint8_t value = checkImage[xc-row-1][col][0];
                        imfile.write((char *)&value, 1);
                    } else {
                        uint8_t red = checkImage[cli.imageData.imageLength-row-1][col][0];
                        uint8_t green = checkImage[cli.imageData.imageLength-row-1][col][1];
                        uint8_t blue = checkImage[cli.imageData.imageLength-row-1][col][2];
                        imfile.write((char *)&red, 1);
                        imfile.write((char *)&green, 1);
                        imfile.write((char *)&blue, 1);
                    }
                }
            }
            afterifd = imfile.tellp();
            imfile.seekp(prevPos, std::ios::beg);

            // SamplesPerPixel
            tag = 277;
            type = 3;
            count = 1;
            if (cli.imageData.isGrayscale) {
                offset = 1;
            } else {
                offset = 3;
            }
            writeIFDEntry(imfile, tag, type, count, offset);

            // RowsPerStrip
            tag = 278;
            type = 4;
            count = 1;
            offset = imageHeight;
            writeIFDEntry(imfile, tag, type, count, offset);

            // StripByteCounts
            tag = 279;
            type = 4;
            count = 1;
            if (cli.imageData.isGrayscale) {
                offset = imageHeight * imageWidth;
            } else {
                offset = imageHeight * imageWidth * 3;
            }
            writeIFDEntry(imfile, tag, type, count, offset);

            // XResolution
            tag = 282;
            type = 5;
            count = 1;
            offset = (uint32_t)afterifd;
            prevPos = imfile.tellp();
            imfile.seekp(afterifd, std::ios::beg);
            uint32_t numer = cli.imageData.xResNum;
            uint32_t denom = cli.imageData.xResDen;
            imfile.write((char *)&numer, 4);
            imfile.write((char *)&denom, 4);
            afterifd = imfile.tellp();
            imfile.seekp(prevPos, std::ios::beg);
            writeIFDEntry(imfile, tag, type, count, offset);

            // YResolution
            tag = 283;
            type = 5;
            count = 1;
            offset = (uint32_t)afterifd;
            prevPos = imfile.tellp();
            imfile.seekp(afterifd, std::ios::beg);
            numer = cli.imageData.yResNum;
            denom = cli.imageData.yResDen;
            imfile.write((char *)&numer, 4);
            imfile.write((char *)&denom, 4);
            afterifd = imfile.tellp();
            imfile.seekp(prevPos, std::ios::beg);
            writeIFDEntry(imfile, tag, type, count, offset);

            // Resolution Unit
            tag = 296;
            type = 3;
            count = 1;
            offset = cli.imageData.resolutionUnit;
            writeIFDEntry(imfile, tag, type, count, offset);

            uint32_t nextifd = 0;
            imfile.write((char *)&nextifd, 4);

            imfile.close();
        } catch (std::exception& ex) {
            std::cout << "Error: could not complete file write" << std::endl;
            imfile.close();
            return;
        }
    } else {
        std::cout << "Error: cannot create file " << fileName << std::endl;
        return;
    }
}

std::string TiffWriteCommand::toString() const {
    return name + " \"" + fileName + "\"";
}

void TiffWriteCommand::writeIFDEntry(std::ofstream &imfile, uint16_t tag, uint16_t type, uint32_t count, uint32_t offset) {
    imfile.write((char *)&tag, 2);
    imfile.write((char *)&type, 2);
    imfile.write((char *)&count, 4);
    imfile.write((char *)&offset, 4);
}
