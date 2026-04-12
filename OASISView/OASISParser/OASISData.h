#ifndef OASISDATA_H
#define OASISDATA_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "OASISCells.h"

namespace OASISParser {

class OASISData
{
private:
    std::unordered_set<unsigned> mLayerSet;
    std::unordered_map<std::string, OASISCell*> mCellMap;  // Key: Cell name or Reference, Value: OASISCell

    std::string mVersion;
    double mUnit;
    unsigned int mOffsetFlag; // 0 : Table offset is in the Start, 1: Table offset is in the End
    TableOffsets mTableOffsets;

    int parseMagicBytes(std::ifstream& fileStream);
    int parseStart(std::ifstream& fileStream);
public:
    OASISData();
    ~OASISData();

    void parse(const std::string& filePath);
    OASISCell* getCell(unsigned reference);
    OASISCell* getCell(std::string cellName);

    std::vector<unsigned> getLayers();
    std::vector<std::string> getAllCellNames();
};

}

#endif // OASISDATA_H
