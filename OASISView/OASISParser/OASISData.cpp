#include "OASISData.h"
#include "OASISElementParser.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

namespace OASISParser {

OASISData::OASISData() {}

OASISData::~OASISData() {
    for (auto& [key, cell] : mCellMap) {
        delete cell;
    }
    mCellMap.clear();
}

// <oasis-file> -> <magic-bytes> START { CBLOCK | PAD | PROPERTY | <cell> | <name> }* END
void OASISData::parse(const string& filePath) {
    ifstream fileStream = ifstream(filePath, std::ios::binary);

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
        case 2:  // End
            if (mOffsetFlag == 1) {
                mTableOffsets.parse(fileStream);
            }
            break;
        case 13: // Cell Record: reference-number, CELL{CBLOCK|PAD|PROPERTY|XYRELATIVE|XVABSOLUTE|<element>}*
        {
            const unsigned referenceNumber = parseUInt(fileStream);
            string key = std::to_string(referenceNumber);
            OASISCell* cell = new OASISCell((*this), referenceNumber);
            cell->parse(fileStream, mLayerSet);
            mCellMap[key] = cell;
            break;
        }
        case 14: // Cell Record: cellname-string, CELL{CBLOCK|PAD|PROPERTY|XYRELATIVE|XVABSOLUTE|<element>}*
        {
            string cellName = parseNString(fileStream);
            OASISCell* cell = new OASISCell((*this), cellName);
            cell->parse(fileStream, mLayerSet);
            mCellMap[cellName] = cell;
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

    for (auto& [key, cell] : mCellMap) {
        cell->calculateBoundingBox();
    }
}

int OASISData::parseMagicBytes(ifstream& fileStream) {
    char buffer[13];

    fileStream.read(buffer, sizeof(buffer));
    return strncmp(buffer, "%SEMI-OASIS\r\n", sizeof(buffer));
}

int OASISData::parseStart(ifstream& fileStream) {
    char code;
    fileStream.read(&code, sizeof(code));
    if (code != 1) {
        return -1;
    }

    mVersion = parseAString(fileStream);
    mUnit = parseRealNumber(fileStream);
    mOffsetFlag = parseUInt(fileStream);

    if (mOffsetFlag == 0) {
        mTableOffsets.parse(fileStream);
    }
    return 0;
}

OASISCell* OASISData::getCell(unsigned reference) {
    string key = std::to_string(reference);
    return mCellMap.at(key);
}

OASISCell* OASISData::getCell(std::string cellName) {
    return mCellMap.at(cellName);
}

vector<unsigned> OASISData::getLayers() {
    vector<unsigned> sortedVector(mLayerSet.begin(), mLayerSet.end());

    sort(sortedVector.begin(), sortedVector.end());
    return sortedVector;
}

vector<std::string> OASISData::getAllCellNames() {
    vector<string> cellNames;

    for (auto& [key, cell] : mCellMap) {
        cellNames.push_back(key);
    }

    return cellNames;
}

}
