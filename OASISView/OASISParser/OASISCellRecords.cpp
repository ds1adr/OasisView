#include "OASISCellRecords.h"
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

PropertyElement::~PropertyElement() {

}

void PropertyElement::parse(ifstream& fileStream) {
    PropertyInfoByte infoByte;  // UUUUVCNS

    fileStream.read((char*)&infoByte, sizeof(char));

    if (infoByte.isReference) {
        mReference = OASISParser::parseUInt(fileStream);
    } else {
        mPropNameString = OASISParser::parseNString(fileStream);
        cout << "Property" << mPropNameString << endl;
    }
    if (!infoByte.isPropertyValue) {
        if (infoByte.numberOfValue == 15) {
            mPropValueCount = OASISParser::parseUInt(fileStream);
        } else {
            mPropValueCount = infoByte.numberOfValue;
        }
        cout << "PropertyValueCount:" << mPropValueCount << endl;
    }
    for (int i = 0; i < mPropValueCount; i++) {
        unsigned int type = OASISParser::parseUInt(fileStream);
        if (type >= 0 && type <= 7) {
            double realNumber = OASISParser::parseRealNumber(fileStream);
            cout << "PropertyValue (real):" << realNumber << endl;
            mProperty.push_back(realNumber);
        }
        switch (type) {
        case 8:
        {
            unsigned int value = OASISParser::parseUInt(fileStream);
            cout << "PropertyValue 8:" << value << endl;
            mProperty.push_back(value);
            break;
        }
        case 9:
        {
            int value = OASISParser::parseInt(fileStream);
            cout << "PropertyValue 9:" << value << endl;
            mProperty.push_back(value);
            break;
        }
        case 10:
        {
            string str = OASISParser::parseAString(fileStream);
            cout << "PropertyValue 10:" << str << endl;
            mProperty.push_back(str);
            break;
        }
        case 11:
        {
            string str = OASISParser::parseBString(fileStream);
            cout << "PropertyValue 11:" << str << endl;
            mProperty.push_back(str);
            break;
        }
        case 12:
        {
            string str = OASISParser::parseNString(fileStream);
            cout << "PropertyValue 12:" << str << endl;
            mProperty.push_back(str);
            break;
        }
        case 13:
        {
            string str = OASISParser::parseAString(fileStream);
            cout << "PropertyValue 13:" << str << endl;
            mProperty.push_back(str);
            break;
        }
        case 14:
        {
            string str = OASISParser::parseBString(fileStream);
            cout << "PropertyValue 14:" << str << endl;
            mProperty.push_back(str);
            break;
        }
        case 15:
        {
            string str = OASISParser::parseNString(fileStream);
            cout << "PropertyValue 15:" << str << endl;
            mProperty.push_back(str);
            break;
        }
        }
    }
}