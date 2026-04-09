#include "OasisCellRecords.h"
#include "OASISElementParser.h"

#include <fstream>
#include <iostream>

using namespace std;

TextElement::~TextElement() {

}

void TextElement::parse(ifstream& fileStream) {
    OASISParser::byte_t infoByte; // 0CNXYRTL

    fileStream.read((char*)&infoByte, sizeof(infoByte));

    if (infoByte & 0x40) {
        mReference = OASISParser::parseUInt(fileStream);
    } else {
        mReference = 0;
        mText = OASISParser::parseAString(fileStream);
        cout << "Text Element:" << mText << endl;
    }
    if (infoByte & 0x01) {
        mTextLayer = OASISParser::parseUInt(fileStream);
        cout << "TextLayer:" << mTextLayer << endl;
    }
    if (infoByte & 0x02) {
        mTextType = OASISParser::parseUInt(fileStream);
        cout << "TextType:" << mTextType << endl;
    }
    if (infoByte & 0x10) {
        mX = OASISParser::parseInt(fileStream);
        cout << "mX:" << mX << endl;
    }
    if (infoByte & 0x08) {
        mY = OASISParser::parseInt(fileStream);
        cout << "mY:" << mY << endl;
    }
    if (infoByte & 0x04) {
        mRepetition = OASISParser::parseRepetition(fileStream);
    }
}