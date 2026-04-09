#include "OasisCellRecords.h"
#include "OASISElementParser.h"

#include <fstream>
#include <iostream>

using namespace std;

TextElement::~TextElement() {

}

void TextElement::parse(ifstream& fileStream) {
    TextInfoByte infoByte; // 0CNXYRTL

    fileStream.read((char*)&infoByte, sizeof(char));

    if (infoByte.isReferenceNumber) {
        mReference = OASISParser::parseUInt(fileStream);
    } else {
        mReference = 0;
        mText = OASISParser::parseAString(fileStream);
        cout << "Text Element:" << mText << endl;
    }
    if (infoByte.isTextLayer) {
        mTextLayer = OASISParser::parseUInt(fileStream);
        cout << "TextLayer:" << mTextLayer << endl;
    }
    if (infoByte.isTextTypeNumber) {
        mTextType = OASISParser::parseUInt(fileStream);
        cout << "TextType:" << mTextType << endl;
    }
    if (infoByte.isX) {
        mX = OASISParser::parseInt(fileStream);
        cout << "mX:" << mX << endl;
    }
    if (infoByte.isY) {
        mY = OASISParser::parseInt(fileStream);
        cout << "mY:" << mY << endl;
    }
    if (infoByte.isRepetition) {
        mRepetition = OASISParser::parseRepetition(fileStream);
    }
}