#include <fstream>
#include <iostream>

#include "EndianChecks.h"
#include "TiffStatCommand.h"

TiffStatCommand::TiffStatCommand(std::string &file) : name("tiffstat"), fileName(file) {
    for (size_t i = 0; i < 33433; i++) {
        tagNames[i] = "";
    }
    tagNames[254] = "NewSubfileType";
    tagNames[255] = "SubfileType";
    tagNames[256] = "ImageWidth";
    tagNames[257] = "ImageLength";
    tagNames[258] = "BitsPerSample";
    tagNames[259] = "Compression";
    tagNames[262] = "PhotometricInterpretation";
    tagNames[263] = "Threshholding";
    tagNames[264] = "CellWidth";
    tagNames[265] = "CellLength";
    tagNames[266] = "FillOrder";
    tagNames[269] = "DocumentName";
    tagNames[270] = "ImageDescription";
    tagNames[271] = "Make";
    tagNames[272] = "Model";
    tagNames[273] = "StripOffsets";
    tagNames[274] = "Orientation";
    tagNames[277] = "SamplesPerPixel";
    tagNames[278] = "RowsPerStrip";
    tagNames[279] = "StripByteCounts";
    tagNames[280] = "MinSampleValue";
    tagNames[281] = "MaxSampleValue";
    tagNames[282] = "XResolution";
    tagNames[283] = "YResolution";
    tagNames[284] = "PlanarConfiguration";
    tagNames[285] = "PageName";
    tagNames[286] = "XPosition";
    tagNames[287] = "YPosition";
    tagNames[288] = "FreeOffsets";
    tagNames[289] = "FreeByteCounts";
    tagNames[290] = "GrayResponseUnit";
    tagNames[291] = "GrayResponseCurve";
    tagNames[292] = "T4Options";
    tagNames[293] = "T6Options";
    tagNames[296] = "ResolutionUnit";
    tagNames[297] = "PageNumber";
    tagNames[301] = "TransferFunction";
    tagNames[305] = "Software";
    tagNames[306] = "DateTime";
    tagNames[315] = "Artist";
    tagNames[316] = "HostComputer";
    tagNames[317] = "Predictor";
    tagNames[318] = "WhitePoint";
    tagNames[319] = "PrimaryChromaticities";
    tagNames[320] = "ColorMap";
    tagNames[321] = "HalftoneHints";
    tagNames[322] = "TileWidth";
    tagNames[323] = "TileLength";
    tagNames[324] = "TileOffsets";
    tagNames[325] = "TileByteCounts";
    tagNames[332] = "InkSet";
    tagNames[333] = "InkNames";
    tagNames[334] = "NumberOfInks";
    tagNames[336] = "DotRange";
    tagNames[337] = "TargetPrinter";
    tagNames[338] = "ExtraSamples";
    tagNames[339] = "SampleFormat";
    tagNames[340] = "SMinSampleValue";
    tagNames[341] = "SMaxSampleValue";
    tagNames[342] = "TransferRange";
    tagNames[512] = "JPEGProc";
    tagNames[513] = "JPEGInterchangeFormat";
    tagNames[514] = "JPEGInterchangeFormatLngth";
    tagNames[515] = "JPEGRestartInterval";
    tagNames[517] = "JPEGLosslessPredictors";
    tagNames[518] = "JPEGPointTransforms";
    tagNames[519] = "JPEGQTables";
    tagNames[520] = "JPEGDCTables";
    tagNames[521] = "JPEGACTables";
    tagNames[529] = "YCbCrCoefficients";
    tagNames[530] = "YCbCrSubSampling";
    tagNames[531] = "YCbCrPositioning";
    tagNames[532] = "ReferenceBlackWhite";
    tagNames[33432] = "Copyright";
}

std::string TiffStatCommand::toString() const {
    return name + " \"" + fileName + "\"";
}

void TiffStatCommand::execute() {
    std::cout << toString() << ":" << std::endl;
    std::ifstream imfile;
    imfile.open(fileName, std::ios::binary);
    if (imfile.is_open()) {
        try {
            bool swap = false;
            bool isBigEndian = false;
            uint16_t endian; // 2 byte quantity
            uint16_t magicNum; // 2 byte quantity
            uint32_t ifdAddress; // 4 byte quantity
            uint16_t ifdCount; // 2 byte quantity

            imfile.seekg (0, std::ios::beg); // not strictly necessary, but a good idea

            // match endian type
            imfile.read((char *)&endian, 2);
            if (endian == 0x4d4d) {
                isBigEndian = true;
            } else if (endian != 0x4949) {
                std::cout << "Error: Invalid TIFF format - cannot determine endianness" <<std::endl;
                imfile.close();
                return;
            }

            if (isBigEndian != is_Big_Endian()) {
                swap = true;
            }

            std::cout << "Endian: 0x" << std::hex << endian << "\n";

            readEndian(imfile, (char *)&magicNum, 2, swap);
            if (magicNum != 42) {
                std::cout << "Error: Not a TIFF file, Magic Number is Incorrect" << std::endl;
                imfile.close();
                return;
            }

            std::cout <<"Magic: " << std::dec << magicNum << "\n";

            readEndian(imfile, (char *)&ifdAddress, 4, swap);

            std::cout << "Directory 0: offset 0x" << std::hex << ifdAddress << " next 0" << std::endl;

            // go to IFD position
            imfile.seekg(ifdAddress, std::ios::beg);
            readEndian(imfile, (char*)&ifdCount, 2, swap);

            for (uint16_t i = 0; i < ifdCount; i++) {
                uint16_t tag;
                uint16_t type;
                uint32_t count;
                readEndian(imfile, (char*)&tag, 2, swap);

                if (tag > 33432 || tagNames[tag] == "") {
                    std::cout << "Custom Tag (" << std::hex << tag << ") ";
                }

                std::cout << tagNames[tag] << " (" << std::dec << tag << ") ";

                readEndian(imfile, (char *)&type, 2, swap);
                readEndian(imfile, (char *)&count, 4, swap);

                if ((type < 3 && count < 5) || (type == 3 && count < 3) || (type == 4 && count == 1)) {
                // values ift on the offset slot
                    if (type == 1) {
                        std::cout << "BYTE (1) " << count << "< ";
                        uint8_t value;
                        for (uint32_t j = 0; j < count; j++) {
                            readEndian(imfile, (char*)&value, 1, swap);
                            std::cout << value << " ";
                        }
                        std::cout << ">" << std::endl;
                        uint32_t junk;
                        readEndian(imfile, (char *)&junk, 4 - count, swap);
                    } else if (type == 2) {
                        std:: cout << "ASCII (2) " << count << "< ";
                        char values[4];
                        readEndian(imfile, values, count, swap);
                        std::cout << values << " >" << std::endl;
                        uint32_t junk;
                        readEndian(imfile, (char *)&junk, 4-count, swap);
                    } else if (type == 3) {
                        std::cout << "SHORT (3) " << count << "< ";
                        uint16_t value;
                        for (uint32_t j = 0; j < count; j++) {
                            readEndian(imfile, (char *)&value, 2, swap);
                            std::cout << value << " ";
                        }
                        std::cout << ">" << std::endl;
                        uint32_t junk;
                        readEndian(imfile, (char *)&junk, 4 - 2*count, swap);
                    } else if (type == 4) {
                        uint32_t value;
                        readEndian(imfile, (char *)&value, 4, swap);
                        std::cout << "Long (4) 1< " << value << " >" << std::endl;
                    }
                } else {
                    // values do not fit on the offset slot
                    uint32_t offset;
                    readEndian(imfile, (char *)&offset, 4, swap);
                    std::streampos prevPos = imfile.tellg();
                    imfile.seekg(offset, std::ios::beg);
                    if (type == 1) {
                        std::cout << "BYTE (1) " << count << "< ";
                        uint8_t value;
                        for (uint32_t j = 0; j < count; j++) {
                            readEndian(imfile, (char *)&value, 1, swap);
                            std::cout << value << " ";
                        }
                        std::cout << ">" << std::endl;
                    } else if (type == 2) {
                        std::cout << "ASCII (2) " << count << "< ";
                        char *values = new char[count];
                        readEndian(imfile, values, count, swap);
                        std::cout << values << " >" << std::endl;
                        delete[] values;
                    } else if (type == 3) {
                        std::cout << "SHORT (3) " << count << "< ";
                        uint16_t value;
                        for (uint32_t j = 0; j < count; j++) {
                            readEndian(imfile, (char *)&value, 2, swap);
                            std::cout << value << " ";
                        }
                        std::cout << ">" << std::endl;
                    } else if (type == 4) {
                        std::cout << "LONG (4) " << count << "< ";
                        uint32_t value;
                        for (uint32_t j = 0; j < count; j++) {
                            readEndian(imfile, (char *)&value, 4, swap);
                            std::cout << value << " ";
                        }
                        std::cout << ">" << std::endl;
                    } else if (type == 5) {
                        std::cout << "RATIONAL (5) " << count << "< ";
                        uint32_t numer;
                        uint32_t denom;
                        for (uint32_t j = 0; j < count; j++) {
                            readEndian(imfile, (char *)&numer, 4, swap);
                            readEndian(imfile, (char *)&denom, 4, swap);
                            if (denom == 0) {
                                std::cout << "NaN" << " ";
                            } else {
                                std::cout << numer/denom << " ";
                            }
                        }
                        std::cout << ">" << std::endl;
                    }
                    imfile.seekg(prevPos, std::ios::beg);
                }
            }
            imfile.close();
        } catch (std::exception ex) {
            imfile.close();
            std::cout << "Error: unable to read file as TIFF format" << std::endl;
        }
    } else {
        std::cout << "Error: invalid filename \"" << fileName << "\"" << std::endl;
    }
}


