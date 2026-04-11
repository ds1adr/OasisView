#ifndef OASISCELLS_H
#define OASISCELLS_H

#include <string>
#include <unordered_set>

#include "OASISCellRecords.h"
#include "OASISElementParser.h"

namespace OASISParser {

struct BoundingBox {
    int lx = INT_MAX, ly = INT_MAX;
    int mx = INT_MIN, my = INT_MIN;
};

struct TableOffsetPair {
    unsigned int flag;
    unsigned int offset;

    int parse(std::ifstream& fileStream) {
        flag = OASISParser::parseUInt(fileStream);
        offset = OASISParser::parseUInt(fileStream);
        return 0;
    }
};

struct TableOffsets {
    TableOffsetPair cellName;
    TableOffsetPair textString;
    TableOffsetPair propName;
    TableOffsetPair propString;
    TableOffsetPair layerName;
    TableOffsetPair xName;

    int parse(std::ifstream& fileStream) {
        cellName.parse(fileStream);
        textString.parse(fileStream);
        propName.parse(fileStream);
        propString.parse(fileStream);
        layerName.parse(fileStream);
        xName.parse(fileStream);
        return 0;
    }
};

class OASISCell
{
private:
    unsigned int mReference = 0;
    std::string mName;
    std::vector<CellElement*> mCellElements;
    BoundingBox mBoundingBox;
public:
    OASISCell(const unsigned int reference);
    OASISCell(const std::string& name);
    ~OASISCell();

    void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet);
    const BoundingBox& getBoundingBox();
};

class OASISCellRef {
public:
    OASISCellRef();
};

}

#endif // OASISCELLS_H
