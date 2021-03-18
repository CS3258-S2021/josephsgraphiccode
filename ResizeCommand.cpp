#include <iostream>
#include <cmath>
#include "ResizeCommand.h"
#include "Global.h"

// Constructor for Resize Command
ResizeCommand::ResizeCommand(std::vector<double>& p, CLI& mainCLI): name("resize"), xScaleFactor(p[0]), yScaleFactor(p[1]), cli(mainCLI) {}

// Constructor for Zoom Command
ResizeCommand::ResizeCommand(double scale, CLI& mainCLI): name("zoom"), xScaleFactor(scale), yScaleFactor(scale), cli(mainCLI) {}

void ResizeCommand::execute() {
    std::cout << toString() << std::endl;

    if (!cli.hasImage) {
        std::cout << "Error: please read an image first" << std::endl;
        return;
    }
    if ((cli.imageData.imageLength * yScaleFactor >= (checkImageHeight + 1)) || (cli.imageData.imageWidth * xScaleFactor >= (checkImageWidth + 1))) {
        std::cout << "Error: cannot scale past size " << checkImageWidth << "x" << checkImageHeight << std::endl;
        return;
    }

    bool xscaleNeg = xScaleFactor < 0;
    bool yscaleNeg = yScaleFactor < 0;
    if (xscaleNeg) {
        xScaleFactor = -xScaleFactor;
    }
    if (yscaleNeg) {
        yScaleFactor = -yScaleFactor;
    }

    resizeX();
    resizeY();

    if (xscaleNeg) {
        invertX();
    }
    if (yscaleNeg) {
        invertY();
    }
    glutPostRedisplay();
}

std::string ResizeCommand::toString() const {
    // Outputting command to user
    if (name == "zoom") {
        return name + " " + std::to_string(xScaleFactor);
    } else {
        return name + " " + std::to_string(xScaleFactor) + "," + std::to_string(yScaleFactor);
    }
}

uint16_t ResizeCommand::sample(int row, int col, uint8_t rgb) {
    if (row < 0 || row >= cli.imageData.imageLength || col < 0 || col >= cli.imageData.imageWidth) {
        if (cli.filter.getBorder() == "zero") {
            return 0;
        } else if (cli.filter.getBorder() == "freeze") {
            if (row < 0) {
                row = 0;
            } else if (row >= cli.imageData.imageLength) {
                row = cli.imageData.imageLength - 1;
            }
            if (col < 0) {
                col = 0;
            } else if (col >= cli.imageData.imageWidth) {
                col = cli.imageData.imageWidth - 1;
            }
        } else if (cli.filter.getBorder() == "circular") {
            if (row < 0) {
                row = (row/cli.imageData.imageLength)%2 == 0 ? cli.imageData.imageLength - 1 - (-row)%cli.imageData.imageLength : (-row)%cli.imageData.imageLength;
            } else if (row >= cli.imageData.imageLength) {
                row = (row/cli.imageData.imageLength)%2 == 0 ? (-row)%cli.imageData.imageLength : cli.imageData.imageLength - 1 - (-row)%cli.imageData.imageLength;
            }
            if (col < 0) {
                col = (col/cli.imageData.imageWidth)%2 == 0 ? cli.imageData.imageWidth - 1 - (-col)%cli.imageData.imageWidth : (-col)%cli.imageData.imageWidth;
            } else if (col >= cli.imageData.imageWidth) {
                col = (col/cli.imageData.imageWidth)%2 == 0 ? (-col)%cli.imageData.imageWidth : cli.imageData.imageWidth - 1 - (-col)%cli.imageData.imageWidth;
            }
        }
    }

    return checkImage[row][col][rgb];
}

void ResizeCommand::resizeX() {
    auto targetWidth = (size_t)(cli.imageData.imageWidth * xScaleFactor);
    std::cout << "New Width: " << targetWidth << std::endl;
    static GLubyte targetImage[checkImageHeight][checkImageWidth][3];

    int row = 0;
    std::vector<std::vector<double>> filterVals(targetWidth, std::vector<double>());
    std::vector<std::vector<int>> pixelsList(targetWidth, std::vector<int>());
    for (int col = 0; col < targetWidth; ++col) {
        std::vector<int> pixels;
        std::vector<double> positions;
        findPixelsInRange(col, xScaleFactor, cli.imageData.imageWidth, targetWidth, yScaleFactor >= 1, pixels, positions);
        std::vector<double> filterVal;
        for (auto position : positions) {
            filterVal.push_back(cli.filter.getValue(position));
        }
        filterVals[col] = filterVal;
        pixelsList[col] = pixels;

        std::vector<double> red, green, blue;
        for (auto pixel : pixels) {
            red.push_back(sample(row, pixel, 0));
            green.push_back(sample(row, pixel, 1));
            blue.push_back(sample(row, pixel, 2));
        }

        double norm = 0;
        double r = 0, g = 0, b = 0;
        for (int i = 0; i < filterVal.size(); ++i) {
            norm += filterVal[i];
            r += filterVal[i] * red[i];
            b += filterVal[i] * blue[i];
            g += filterVal[i] * green[i];
        }
        targetImage[row][col][0] = r/norm > 255 ? (GLubyte)255 : (GLubyte)(r/norm);
        targetImage[row][col][1] = g/norm > 255 ? (GLubyte)255 : (GLubyte)(g/norm);
        targetImage[row][col][2] = b/norm > 255 ? (GLubyte)255 : (GLubyte)(b/norm);
    }


    for (;row < cli.imageData.imageLength; ++row) {
        for (int col = 1; col < targetWidth; ++col) {
            std::vector<int> pixels = pixelsList[col];
            std::vector<double> filterVal = filterVals[col];

            std::vector<double> red, green, blue;
            for (auto pixel : pixels) {
                red.push_back(sample(row, pixel, 0));
                green.push_back(sample(row, pixel, 1));
                blue.push_back(sample(row, pixel, 2));
            }

            double norm = 0;
            double r = 0, g = 0, b = 0;
            for (int i = 0; i < filterVal.size(); ++i) {
                norm += filterVal[i];
                r += filterVal[i] * red[i];
                b += filterVal[i] * blue[i];
                g += filterVal[i] * green[i];
            }
            targetImage[row][col][0] = r/norm > 255 ? (GLubyte)255 : r/norm < 0 ? (GLubyte)0 : (GLubyte)(r/norm);
            targetImage[row][col][1] = g/norm > 255 ? (GLubyte)255 : g/norm < 0 ? (GLubyte)0 : (GLubyte)(g/norm);
            targetImage[row][col][2] = b/norm > 255 ? (GLubyte)255 : b/norm < 0 ? (GLubyte)0 : (GLubyte)(b/norm);
        }
    }

    // Clears image buffer
    makeCheckImage();

    // Copy the new image to the image buffer
    for (int r = 0; r < cli.imageData.imageLength; ++r) {
        for (int c = 0; c < targetWidth; ++c) {
            for (int rgb = 0; rgb < 3; ++rgb) {
                checkImage[r][c][rgb] = targetImage[r][c][rgb];
            }
        }
    }

    cli.imageData.imageWidth = (uint32_t)targetWidth;
}

void ResizeCommand::resizeY() {
    auto targetHeight = (size_t)(cli.imageData.imageLength * yScaleFactor);
    std::cout << "New Height: " << targetHeight << std::endl;
    static GLubyte targetImage[checkImageHeight][checkImageWidth][3];

    int col = 0;
    std::vector<std::vector<double>> filterVals(targetHeight, std::vector<double>());
    std::vector<std::vector<int>> pixelsList(targetHeight, std::vector<int>());
    for (int row = 0; row < targetHeight; ++row) {
        std::vector<int> pixels;
        std::vector<double> positions;
        findPixelsInRange(row, yScaleFactor, cli.imageData.imageLength, targetHeight, yScaleFactor >= 1, pixels, positions);
        std::vector<double> filterVal;
        for (auto position : positions) {
            filterVal.push_back(cli.filter.getValue(position));
        }
        filterVals[row] = filterVal;
        pixelsList[row] = pixels;

        std::vector<double> red, green, blue;
        for (auto pixel : pixels) {
            red.push_back(sample(pixel, col, 0));
            green.push_back(sample(pixel, col, 1));
            blue.push_back(sample(pixel, col, 2));
        }

        double norm = 0;
        double r = 0, g = 0, b = 0;
        for (int i = 0; i < filterVal.size(); ++i) {
            norm += filterVal[i];
            r += filterVal[i] * red[i];
            b += filterVal[i] * blue[i];
            g += filterVal[i] * green[i];
        }
        targetImage[row][col][0] = r/norm > 255 ? (GLubyte)255 : (GLubyte)(r/norm);
        targetImage[row][col][1] = g/norm > 255 ? (GLubyte)255 : (GLubyte)(g/norm);
        targetImage[row][col][2] = b/norm > 255 ? (GLubyte)255 : (GLubyte)(b/norm);
    }


    for (;col < cli.imageData.imageWidth; ++col) {
        for (int row = 1; row < targetHeight; ++row) {
            std::vector<int> pixels = pixelsList[row];
            std::vector<double> filterVal = filterVals[row];

            std::vector<double> red, green, blue;
            for (auto pixel : pixels) {
                red.push_back(sample(pixel, col, 0));
                green.push_back(sample(pixel, col, 1));
                blue.push_back(sample(pixel, col, 2));
            }

            double norm = 0;
            double r = 0, g = 0, b = 0;
            for (int i = 0; i < filterVal.size(); ++i) {
                norm += filterVal[i];
                r += filterVal[i] * red[i];
                b += filterVal[i] * blue[i];
                g += filterVal[i] * green[i];
            }
            targetImage[row][col][0] = r/norm > 255 ? (GLubyte)255 : r/norm < 0 ? (GLubyte)0 : (GLubyte)(r/norm);
            targetImage[row][col][1] = g/norm > 255 ? (GLubyte)255 : g/norm < 0 ? (GLubyte)0 : (GLubyte)(g/norm);
            targetImage[row][col][2] = b/norm > 255 ? (GLubyte)255 : b/norm < 0 ? (GLubyte)0 : (GLubyte)(b/norm);
        }
    }

    // Clears the image buffer
    makeCheckImage();

    // Copy the new image to the image buffer
    for (int r = 0; r < targetHeight; ++r) {
        for (int c = 0; c < cli.imageData.imageWidth; ++c) {
            for (int rgb = 0; rgb < 3; ++rgb) {
                checkImage[r][c][rgb] = targetImage[r][c][rgb];
            }
        }
    }

    cli.imageData.imageLength = (uint32_t)targetHeight;
}

void ResizeCommand::invertX() const {
    static GLubyte newImage[checkImageHeight][checkImageWidth][3];
    for (uint32_t row = 0; row < cli.imageData.imageLength; ++row) {
        for (uint32_t col = 0; col < cli.imageData.imageWidth; ++col) {
            for (int rgb = 0; rgb < 3; ++rgb) {
                newImage[row][cli.imageData.imageWidth - 1 - col][rgb] = checkImage[row][col][rgb];
            }
        }
    }

    // Copy the new image to the image buffer
    for (int r = 0; r < cli.imageData.imageLength; ++r) {
        for (int c = 0; c < cli.imageData.imageWidth; ++c) {
            for (int rgb = 0; rgb < 3; ++rgb) {
                checkImage[r][c][rgb] = newImage[r][c][rgb];
            }
        }
    }
}

void ResizeCommand::invertY() const {
    static GLubyte newImage[checkImageHeight][checkImageWidth][3];
    for (uint32_t row = 0; row < cli.imageData.imageLength; ++row) {
        for (uint32_t col = 0; col < cli.imageData.imageWidth; ++col) {
            for (int rgb = 0; rgb < 3; ++rgb) {
                newImage[cli.imageData.imageLength - 1 - row][col][rgb] = checkImage[row][col][rgb];
            }
        }
    }

    // Copy the new image to the image buffer
    for (int r = 0; r < cli.imageData.imageLength; ++r) {
        for (int c = 0; c < cli.imageData.imageWidth; ++c) {
            for (int rgb = 0; rgb < 3; ++rgb) {
                checkImage[r][c][rgb] = newImage[r][c][rgb];
            }
        }
    }
}

void ResizeCommand::findPixelsInRange(double targetPoint, double factor, double sourceWidth, double targetWidth, bool magnify, std::vector<int>& pixels, std::vector<double>& positions) {
    if (magnify) {
        double centerSource = targetPoint/factor;
        double lowerBoundSource = centerSource - cli.filter.getWidth();
        for (int pixel = (int)ceil(lowerBoundSource); pixel <= lowerBoundSource + 2*cli.filter.getWidth(); ++pixel) {
            pixels.emplace_back(pixel);
        }
        for (auto pixel : pixels) {
            positions.push_back(pixel - centerSource);
        }
    } else {
        double lowerBoundSource = (targetPoint - cli.filter.getWidth()) / factor;
        for (int pixel = (int)ceil(lowerBoundSource); pixel <= lowerBoundSource + 2*cli.filter.getWidth()/factor; ++pixel) {
            pixels.emplace_back(pixel);
        }
        for (auto pixel : pixels) {
            positions.push_back(pixel*factor - targetPoint);
        }
    }
}