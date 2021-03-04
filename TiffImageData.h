#ifndef TIFFIMAGEDATA_H
#define TIFFIMAGEDATA_H

#include <cstdint>

class TiffImageData {
public:
    bool isGrayscale;
    uint32_t imageWidth;
    uint32_t imageLength;
    uint16_t bitsPerSample[3];
    uint16_t compression;
    uint16_t photometric;
    uint32_t *stripOffsets;
    uint32_t numStrips;
    uint16_t samplesPerPixel;
    uint32_t rowsPerStrip;
    uint32_t *stripByteCounts;
    uint32_t xResNum, xResDen;
    uint32_t yResNum, yResDen;
    uint16_t resolutionUnit;

    TiffImageData();
    ~TiffImageData();

    void setStripOffsets(uint32_t values[]);
    void setStripByteCounts(uint32_t values[]);
};


#endif // TIFFIMAGEDATA_H
