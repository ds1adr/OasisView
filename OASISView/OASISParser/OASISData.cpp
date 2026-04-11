#include "OASISData.h"
#include "OASISElementParser.h"

#include <fstream>
#include <iostream>

using namespace std;

namespace OASISParser {

OASISData::OASISData() {}

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
            cout << "End" << endl;
            if (mOffsetFlag == 1) {
                mTableOffsets.parse(fileStream);
            }
        case 13: // Cell Record: reference-number, CELL{CBLOCK|PAD|PROPERTY|XYRELATIVE|XVABSOLUTE|<element>}*
        {
            unsigned int referenceNumber = parseUInt(fileStream);
            string key = std::to_string(referenceNumber);
            cout << "Cell Reference:" << key << endl;
            OASISCell cell(key);
            cell.parse(fileStream, mLayerSet);
            mCellMap.emplace(key, cell);
            break;
        }
        case 14: // Cell Record: cellname-string, CELL{CBLOCK|PAD|PROPERTY|XYRELATIVE|XVABSOLUTE|<element>}*
        {
            string cellName = parseNString(fileStream);
            cout << "Cell Name:" << cellName << endl;
            OASISCell cell(cellName);
            cell.parse(fileStream, mLayerSet);
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
    cout << "Version:" << mVersion << endl;
    mUnit = parseRealNumber(fileStream);
    cout << "Unit:" << mUnit << endl;
    mOffsetFlag = parseUInt(fileStream);
    cout << "Offset Flag:" << mOffsetFlag << endl;
    if (mOffsetFlag == 0) {
        mTableOffsets.parse(fileStream);
    }
    return 0;
}

}