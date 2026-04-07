#include "OASISData.h"
#include "OASISElementParser.h"

#include <fstream>
#include <iostream>

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

    // while (!fileStream.eof()) {

    // }

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
    return 0;
}