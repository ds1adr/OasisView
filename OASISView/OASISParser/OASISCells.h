#ifndef OASISCELLS_H
#define OASISCELLS_H

#include <string>

#include "OASISElementParser.h"

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
    std::string mName;
public:
    OASISCell();
};

class OASISCellRef {
public:
    OASISCellRef();
};

#endif // OASISCELLS_H
