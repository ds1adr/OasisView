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
        cout << "Text Reference:" << mReference << endl;
    } else {
        mReference = 0;
        mText = OASISParser::parseAString(fileStream);
        cout << "Text Element:" << mText << endl;
    }
    if (infoByte.isTextLayer) {
        mTextLayer = OASISParser::parseUInt(fileStream);
    }
    if (infoByte.isTextTypeNumber) {
        mTextType = OASISParser::parseUInt(fileStream);
    }
    if (infoByte.isX) {
        mX = OASISParser::parseInt(fileStream);
    }
    if (infoByte.isY) {
        mY = OASISParser::parseInt(fileStream);
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
    }
    for (int i = 0; i < mPropValueCount; i++) {
        unsigned int type = OASISParser::parseUInt(fileStream);
        if (type >= 0 && type <= 7) {
            double realNumber = OASISParser::parseRealNumber(fileStream);
            mProperty.push_back(realNumber);
        }
        switch (type) {
        case 8:
        {
            unsigned int value = OASISParser::parseUInt(fileStream);
            mProperty.push_back(value);
            break;
        }
        case 9:
        {
            int value = OASISParser::parseInt(fileStream);
            mProperty.push_back(value);
            break;
        }
        case 10:
        {
            string str = OASISParser::parseAString(fileStream);
            mProperty.push_back(str);
            break;
        }
        case 11:
        {
            string str = OASISParser::parseBString(fileStream);
            mProperty.push_back(str);
            break;
        }
        case 12:
        {
            string str = OASISParser::parseNString(fileStream);
            mProperty.push_back(str);
            break;
        }
        case 13:
        {
            string str = OASISParser::parseAString(fileStream);
            mProperty.push_back(str);
            break;
        }
        case 14:
        {
            string str = OASISParser::parseBString(fileStream);
            mProperty.push_back(str);
            break;
        }
        case 15:
        {
            string str = OASISParser::parseNString(fileStream);
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

        cout << "Placement, code:" << mCode << endl;

        if (infoByte.isFlip) {
            mIsFlip = true;
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
        if (infoByte.isReference) {
            mReference = parseUInt(fileStream);
        } else {
            mCellName = parseNString(fileStream);
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
    } else {
        MPlacementInfoByte infoByte;

        fileStream.read((char*)&infoByte, sizeof(char));

        if (infoByte.isFlip) {
            mIsFlip = true;
        }
        if (infoByte.isReference) {
            mReference = parseUInt(fileStream);
        } else {
            mCellName = parseNString(fileStream);
        }
        if (infoByte.isMag) {
            mMag = parseRealNumber(fileStream);
        }
        if (infoByte.isAngle) {
            mRotation = parseRealNumber(fileStream);
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
    }
}

BoundingBox Placement::calculateBoundingBox(OASISData& oasisData) {
    OASISCell* subCell = getCellName().empty() ? oasisData.getCell(getReference()) : oasisData.getCell(getCellName());
    BoundingBox bBox;
    if (subCell->isValid()) {
        BoundingBox subBBox = subCell->calculateBoundingBox();
        if (subBBox.minX == INT_MAX) {
            subBBox = subCell->calculateBoundingBox();
        }
        // TODO: Handle Rotation

        bBox.minX = min(bBox.minX, subBBox.minX + mX);
        bBox.minY = min(bBox.minY, subBBox.minY + mY);

        // Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition
        if (holds_alternative<Repetition>(mRepetition)) {
            Repetition r = get<Repetition>(mRepetition);

            int mx = mX + subBBox.maxX + r.dx * (r.nx - 1);
            int my = mY + subBBox.maxY + r.dy * (r.ny - 1);
            bBox.maxX = max(bBox.maxX, mx);
            bBox.maxY = max(bBox.maxY, my);
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

    cout << "Rectangle" << endl;

    if (infoByte.isLayerNumber) {
        mLayerNumber = OASISParser::parseUInt(fileStream);
        layerSet.insert(mLayerNumber);
    }
    if (infoByte.isDataType) {
        mDataType = OASISParser::parseUInt(fileStream);
    }
    if (infoByte.isWidth) {
        mWidth = OASISParser::parseUInt(fileStream);
    }
    if (infoByte.isSquare) {
        mHeight = mWidth;
    } else if (infoByte.isHeight) {
        mHeight = OASISParser::parseUInt(fileStream);
    }
    if (infoByte.isX) {
        mX = OASISParser::parseInt(fileStream);
    } else {
        mX = 0;
    }
    if (infoByte.isY) {
        mY = OASISParser::parseInt(fileStream);
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

    cout << "Trapezoid" << endl;

    if (infoByte.isLayerNumber) {
        mLayerNumber = parseUInt(fileStream);
        layerSet.insert(mLayerNumber);
    }
    if (infoByte.isDataType) {
        mDataType = parseUInt(fileStream);
    }
    if (infoByte.isWidth) {
        mWidth = parseUInt(fileStream);
    }
    if (infoByte.isHeight) {
        mHeight = parseUInt(fileStream);
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

    cout << "CTrapezoid" << endl;

    if (infoByte.isLayerNumber) {
        mLayerNumber = parseUInt(fileStream);
        layerSet.insert(mLayerNumber);
    }
    if (infoByte.isDataType) {
        mDataType = parseUInt(fileStream);
    }
    if (infoByte.isType) {
        mType = parseUInt(fileStream);
    }
    if (infoByte.isWidth) {
        mWidth = parseUInt(fileStream);
    }
    if (infoByte.isHeight) {
        mHeight = parseInt(fileStream);
    }
    if (infoByte.isX) {
        mX = parseUInt(fileStream);
    }
    if (infoByte.isY) {
        mY = parseUInt(fileStream);
    }
    if (infoByte.isRepetition) {
        mRepetition = parseRepetition(fileStream);
    }
}

}