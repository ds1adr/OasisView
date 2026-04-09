#include "OASISData.h"
#include "OASISElementParser.h"

#include <fstream>
#include <iostream>

namespace OASISParser {

OASISData::OASISData() {}

// <oasis-file> -> <magic-bytes> START { CBLOCK | PAD | PROPERTY | <cell> | <name> }* END
void OASISData::parse(const std::string& filePath) {
    std::ifstream fileStream = std::ifstream(filePath, std::ios::binary);

    if (!fileStream.is_open()) {
        // throw error
        return;
    }

    int result = parseMagicBytes(fileStream);
    if (result != 0) { return; }
    result = parseStart(fileStream);
    if (result != 0) { return; }

    // { CBLOCK | PAD | PROPERTY | <cell> | <name> }*
    while (!fileStream.eof()) {
        char recordType;
        bool quit = false;
        fileStream.read(&recordType, sizeof(recordType));

        switch (recordType) {
        case 13: // Cell Record: reference-number, CELL{CBLOCK|PAD|PROPERTY|XYRELATIVE|XVABSOLUTE|<element>}*
        {
            unsigned int referenceNumber = OASISParser::parseUInt(fileStream);
            std::string key = std::to_string(referenceNumber);
            std::cout << "Cell Reference:" << key << std::endl;
            OASISCell cell(key);
            cell.parse(fileStream);
            mCellMap.emplace(key, cell);
            break;
        }
        case 14: // Cell Record: cellname-string, CELL{CBLOCK|PAD|PROPERTY|XYRELATIVE|XVABSOLUTE|<element>}*
        {
            std::string cellName = OASISParser::parseNString(fileStream);
            std::cout << "Cell Name:" << cellName << std::endl;
            OASISCell cell(cellName);
            cell.parse(fileStream);
            mCellMap.emplace(cellName, cell);
            break;
        }
        default:
            quit = true;
            break;
        }

        // Temporal to prevent infinite loop
        if (quit) {
            break;
        }
    }

    fileStream.close();
}

int OASISData::parseMagicBytes(std::ifstream& fileStream) {
    char buffer[13];

    fileStream.read(buffer, sizeof(buffer));
    return std::strncmp(buffer, "%SEMI-OASIS\r\n", sizeof(buffer));
}

int OASISData::parseStart(std::ifstream& fileStream) {
    char code;
    fileStream.read(&code, sizeof(code));
    if (code != 1) {
        return -1;
    }

    mVersion = OASISParser::parseAString(fileStream);
    std::cout << "Version:" << mVersion << std::endl;
    mUnit = OASISParser::parseRealNumber(fileStream);
    std::cout << "Unit:" << mUnit << std::endl;
    mOffsetFlag = OASISParser::parseUInt(fileStream);
    std::cout << "Offset Flag:" << mOffsetFlag << std::endl;
    if (mOffsetFlag == 0) {
        mTableOffsets.parse(fileStream);
    }
    return 0;
}

}