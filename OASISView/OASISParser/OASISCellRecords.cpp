#include "OASISCellRecords.h"
#include "OASISElement.h"
#include "OASISElementParser.h"
#include "OASISCells.h"
#include "OASISData.h"

#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

namespace OASISParser {

Text::~Text() {

}

void Text::parse(ifstream& fileStream, unordered_set<unsigned>& layerSet) {
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

Property::~Property() {

}

void Property::setValues(Property* origin) {
    mReference = origin->getReference();
    mStandard =  origin->getStandard();;
    mPropNameString = origin->getPropName();
    mPropValueCount = origin->getPropValueCount();
    mProperty = origin->getProperty();
}

void Property::parse(ifstream& fileStream, unordered_set<unsigned>& layerSet) {
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

Placement::~Placement() {

}

void Placement::parse(ifstream& fileStream, unordered_set<unsigned>& layerSet) {
    if (mCode == 17) {
        PlacementInfoByte infoByte;

        fileStream.read((char*)&infoByte, sizeof(char));

        if (infoByte.isFlip) {
            mIsFlip = true;
            cout << "Placement: isFlip" << endl;
        }
        switch (infoByte.rotation) {
        case 0:
            mRotation = 0;
            break;
        case 1:
            mRotation = M_PI / 2;
            break;
        case 2:
            mRotation = M_PI;
            break;
        case 3:
            mRotation = -M_PI / 2;
        }
        cout << "Rotation:" << mRotation << endl;
        if (infoByte.isReference) {
            mReference = parseUInt(fileStream);
        } else {
            mCellName = parseNString(fileStream);
            cout << "Cell name:" << mCellName << endl;
        }
        if (infoByte.isX) {
            mX = parseInt(fileStream);
            cout << "X:" << mX << endl;
        }
        if (infoByte.isY) {
            mY = parseInt(fileStream);
            cout << "Y:" << mY << endl;
        }
        if (infoByte.isRepetition) {
            mRepetition = parseRepetition(fileStream);
            cout << "Repetition" << endl;
        }
    } else {
        MPlacementInfoByte infoByte;

        fileStream.read((char*)&infoByte, sizeof(char));

        if (infoByte.isFlip) {
            mIsFlip = true;
            cout << "Placement: isFlip" << endl;
        }
        if (infoByte.isReference) {
            mReference = parseUInt(fileStream);
        } else {
            mCellName = parseNString(fileStream);
            cout << "Cell name:" << mCellName << endl;
        }
        if (infoByte.isMag) {
            mMag = parseRealNumber(fileStream);
        }
        if (infoByte.isAngle) {
            mRotation = parseRealNumber(fileStream);
            cout << "Rotation:" << mRotation << endl;
        }
        if (infoByte.isX) {
            mX = parseInt(fileStream);
            cout << "X:" << mX << endl;
        }
        if (infoByte.isY) {
            mY = parseInt(fileStream);
            cout << "Y:" << mY << endl;
        }
        if (infoByte.isRepetition) {
            mRepetition = parseRepetition(fileStream);
            cout << "Repetition" << endl;
        }
    }
}

BoundingBox Placement::calculateBoundingBox(OASISData& oasisData) {
    OASISCell* subCell = getCellName().empty() ? oasisData.getCell(getReference()) : oasisData.getCell(getCellName());
    BoundingBox bBox;
    if (subCell->isValid()) {
        BoundingBox subBBox = subCell->calculateBoundingBox();
        if (subBBox.lx == INT_MAX) {
            subBBox = subCell->calculateBoundingBox();
        }
        // TODO: Handle Rotation

        bBox.lx = min(bBox.lx, subBBox.lx + mX);
        bBox.ly = min(bBox.ly, subBBox.ly + mY);

        // Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition
        if (holds_alternative<Repetition>(mRepetition)) {
            Repetition r = get<Repetition>(mRepetition);

            int mx = mX + subBBox.mx + r.dx * (r.nx - 1);
            int my = mY + subBBox.my + r.dy * (r.ny - 1);
            bBox.mx = max(bBox.my, mx);
            bBox.my = max(bBox.my, my);
            return bBox;
        }
    }
    return BoundingBox();
}

Rectangle::~Rectangle() {

}

void Rectangle::parse(ifstream& fileStream, unordered_set<unsigned>& layerSet) {
    RectangleInfoByte infoByte;  // SWHXYRDL

    fileStream.read((char*)&infoByte, sizeof(char));

    if (infoByte.isLayerNumber) {
        mLayerNumber = OASISParser::parseUInt(fileStream);
        layerSet.insert(mLayerNumber);
        cout << "Rect Layer:" << mLayerNumber << endl;
    }
    if (infoByte.isDataType) {
        mDataType = OASISParser::parseUInt(fileStream);
        cout << "Rect Data type:" << mDataType << endl;
    }
    if (infoByte.isWidth) {
        mWidth = OASISParser::parseUInt(fileStream);
        cout << "Rect Width" << mWidth << endl;
    }
    if (infoByte.isSquare) {
        mHeight = mWidth;
        cout << "Rect Height:" << mHeight << endl;
    } else if (infoByte.isHeight) {
        mHeight = OASISParser::parseUInt(fileStream);
        cout << "Rect Height:" << mHeight << endl;
    }
    if (infoByte.isX) {
        mX = OASISParser::parseInt(fileStream);
        cout << "Rect X:" << mX << endl;
    } else {
        mX = 0;
    }
    if (infoByte.isY) {
        mY = OASISParser::parseInt(fileStream);
        cout << "Rect Y" << mY << endl;
    } else {
        mY = 0;
    }
    if (infoByte.isRepetation) {
        mRepetition = OASISParser::parseRepetition(fileStream);
    }
}

Trapezoid::Trapezoid(unsigned int code): mCode(code) {

}

Trapezoid::~Trapezoid() {

}

void Trapezoid::parse(ifstream& fileStream, unordered_set<unsigned>& layerSet) {
    TrapInfoByte infoByte;  // OWHXYRDL

    fileStream.read((char*)&infoByte, sizeof(char));

    if (infoByte.isLayerNumber) {
        mLayerNumber = parseUInt(fileStream);
        layerSet.insert(mLayerNumber);
        cout << "Trapzoid LayerNumber:" << mLayerNumber << endl;
    }
    if (infoByte.isDataType) {
        mDataType = parseUInt(fileStream);
         cout << "Trapzoid DataType:" << mDataType << endl;
    }
    if (infoByte.isWidth) {
        mWidth = parseUInt(fileStream);
        cout << "Trapzoid width:" << mWidth << endl;
    }
    if (infoByte.isHeight) {
        mHeight = parseUInt(fileStream);
        cout << "Trapzoid height:" << mHeight << endl;
    }
    if (mCode == 23) {
        mDeltaA = parseInt(fileStream);
        mDeltaB = parseInt(fileStream);
    } else if (mCode == 24) {
        mDeltaA = parseInt(fileStream);
    } else if (mCode == 25) {
        mDeltaB = parseInt(fileStream);
    }
    if (infoByte.isX) {
        mX = parseInt(fileStream);
    }
    if (infoByte.isY) {
        mY = parseInt(fileStream);
    }
    if (infoByte.isRepetition) {
        mRepetition = parseRepetition(fileStream);
    }
    mOrientation = infoByte.isVertical ? Orientation::Vertical : Orientation::Horizontal;
}

CTrapezoid::~CTrapezoid() {

}

void CTrapezoid::parse(ifstream& fileStream, unordered_set<unsigned>& layerSet) {
    CTrapInfoByte infoByte;  // TWHXYRDL

    fileStream.read((char*)&infoByte, sizeof(char));

    if (infoByte.isLayerNumber) {
        mLayerNumber = parseUInt(fileStream);
        layerSet.insert(mLayerNumber);
        cout << "CTrapezoid Layer:" << mLayerNumber << endl;
    }
    if (infoByte.isDataType) {
        mDataType = parseUInt(fileStream);
        cout << "CTrapezoid datatype:" << mDataType << endl;
    }
    if (infoByte.isType) {
        mType = parseUInt(fileStream);
        cout << "CTrapezoid type:" << mType << endl;
    }
    if (infoByte.isWidth) {
        mWidth = parseUInt(fileStream);
        cout << "CTrapezoid width:" << mWidth << endl;
    }
    if (infoByte.isHeight) {
        mHeight = parseInt(fileStream);
        cout << "CTrapezoid Height:" << mHeight << endl;
    }
    if (infoByte.isX) {
        mX = parseUInt(fileStream);
        cout << "CTrapezoid X:" << mX << endl;
    }
    if (infoByte.isY) {
        mY = parseUInt(fileStream);
        cout << "CTrapezoid Y:" << mY << endl;
    }
    if (infoByte.isRepetition) {
        mRepetition = parseRepetition(fileStream);
    }
}

}