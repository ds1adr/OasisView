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
    OASISData& mOASISData;
    unsigned int mReference = 0;
    std::string mName;
    std::vector<CellElement*> mCellElements;
    BoundingBox mBoundingBox;
public:
    OASISCell(OASISData& oasisData, unsigned reference);
    OASISCell(OASISData& oasisData, std::string& name);
    ~OASISCell();

    bool isValid() { return (mReference > 0 || !mName.empty()); }

    void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet);
    const BoundingBox& calculateBoundingBox();
};

}

#endif // OASISCELLS_H
