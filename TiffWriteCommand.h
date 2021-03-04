#ifndef TIFFWRITECOMMAND_H
#define TIFFWRITECOMMAND_H

#include <string>
#include <vector>
#include <fstream>

#include "Command.h"
#include "CLI.h"

class TiffWriteCommand : public Command {
public:
    TiffWriteCommand(std::string& file, std::vector<double>& p, CLI& mainCLI);
    virtual void execute();
    virtual std::string toString() const;
    virtual ~TiffWriteCommand() {};
private:
    void writeIFDEntry(std::ofstream &imfile, uint16_t tag, uint16_t type, uint32_t count, uint32_t offset);

    std::string name;
    std::string fileName;
    std::vector<double> params;
    CLI& cli;
};


#endif // TIFFWRITECOMMAND_H
