#ifndef RESIZECOMMAND_H
#define RESIZECOMMAND_H

#include <vector>
#include "Command.h"
#include "CLI.h"

class ResizeCommand : public Command {
public:
    ResizeCommand(std::vector<double>& p, CLI& mainCLI);
    ResizeCommand(double scale, CLI& mainCLI);
    void execute() override;
    std::string toString() const override;
    virtual ~ResizeCommand() {}

private:
    std::string name;
    double xScaleFactor, yScaleFactor;
    CLI& cli;

    // gets value of sample image at given position
    uint16_t sample(int row, int col, uint8_t rgb);

    void resizeX();
    void resizeY();
    void invertX() const;
    void invertY() const;

    void findPixelsInRange(double target, double factor, double origWidth, double targWidth, bool magnify, std::vector<int>& pixels, std::vector<double>& positions);
};


#endif // RESIZECOMMAND_H
