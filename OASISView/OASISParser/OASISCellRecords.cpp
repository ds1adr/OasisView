#include "OASISCellRecords.h"
#include "OASISElement.h"
#include "OASISElementParser.h"
#include "OASISCells.h"
#include "OASISData.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

namespace OASISParser {

unsigned _previousWidth = 0;
unsigned _previousHeight = 0;
int _previousX = 0;
int _previousY = 0;

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
            _previousX = mX;
        } else {
            mX = _previousX;
        }
        if (infoByte.isY) {
            mY = parseInt(fileStream);
            _previousY = mY;
        } else {
            mY = _previousY;
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
            _previousX = mX;
        } else {
            mX = _previousX;
        }
        if (infoByte.isY) {
            mY = parseInt(fileStream);
            _previousY = mY;
        } else {
            mY = _previousY;
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
        // TODO: Other Repetition Type
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
        _previousWidth = mWidth;
    } {
        mWidth = _previousWidth;
    }
    if (infoByte.isSquare) {
        mHeight = mWidth;
    } else if (infoByte.isHeight) {
        mHeight = OASISParser::parseUInt(fileStream);
        _previousHeight = mHeight;
    } else {
        mHeight = _previousHeight;
    }
    if (infoByte.isX) {
        mX = OASISParser::parseInt(fileStream);
        _previousX = mX;
    } else {
        mX = _previousX;
    }
    if (infoByte.isY) {
        mY = OASISParser::parseInt(fileStream);
        _previousY = mY;
    } else {
        mY = _previousY;
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
        _previousWidth = mWidth;
    } else {
        mWidth = _previousWidth;
    }
    if (infoByte.isHeight) {
        mHeight = parseUInt(fileStream);
        _previousHeight = mHeight;
    } else {
        mHeight = _previousHeight;
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
        _previousX = mX;
    } else {
        mX = _previousX;
    }
    if (infoByte.isY) {
        mY = parseInt(fileStream);
        _previousY = mY;
    } else {
        mY = _previousY;
    }
    if (infoByte.isRepetition) {
        mRepetition = parseRepetition(fileStream);
    }
    mOrientation = infoByte.isVertical ? Orientation::Vertical : Orientation::Horizontal;
}

const vector<KPoint> Trapezoid::getPoints() {
    vector<KPoint> result;

    if (mOrientation == Orientation::Horizontal) {
        if (mDeltaA >= 0) {
            KPoint p1 = KPoint(mX, mY);
            result.push_back(p1);
            KPoint p2 = KPoint(mX + mWidth, mY);
            result.push_back(p2);
            KPoint p3 = KPoint(mX + mWidth + mDeltaB, mY + mHeight);
            result.push_back(p3);
            KPoint p4 = KPoint(mX + mDeltaA, mY + mHeight);
            result.push_back(p4);
        } else {
            KPoint p1 = KPoint(mX - mDeltaA, mY);
            result.push_back(p1);
            KPoint p2 = KPoint(mX + mWidth - mDeltaB, mY);
            result.push_back(p2);
            KPoint p3 = KPoint(mX + mWidth, mY + mHeight);
            result.push_back(p3);
            KPoint p4 = KPoint(mX, mY + mHeight);
            result.push_back(p4);
        }
    } else { // Vertical
        if (mDeltaA >= 0) {
            KPoint p1 = KPoint(mX, mY + mDeltaA);
            result.push_back(p1);
            KPoint p2 = KPoint(mX + mWidth, mY);
            result.push_back(p2);
            KPoint p3 = KPoint(mX + mWidth, mY + mHeight);
            result.push_back(p3);
            KPoint p4 = KPoint(mX, mY + mHeight + mDeltaB);
            result.push_back(p4);
        } else {
            KPoint p1 = KPoint(mX, mY);
            result.push_back(p1);
            KPoint p2 = KPoint(mX + mWidth, mY - mDeltaA);
            result.push_back(p2);
            KPoint p3 = KPoint(mX + mWidth, mY + mHeight - mDeltaB);
            result.push_back(p3);
            KPoint p4 = KPoint(mX, mY + mHeight);
            result.push_back(p4);
        }
    }
    return result;
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
        cout << "CTrapezoid Layer:" << mLayerNumber << endl;
    }
    if (infoByte.isDataType) {
        mDataType = parseUInt(fileStream);
        cout << "CTrapzoid Data type:" << mDataType << endl;
    }
    if (infoByte.isType) {
        mType = parseUInt(fileStream);
        cout << "CTrapezoid Type:" << mType << endl;
    }
    if (infoByte.isWidth) {
        mWidth = parseUInt(fileStream);
        cout << "CTrapezoid width:" << mWidth << endl;
    }
    if (infoByte.isHeight) {
        mHeight = parseUInt(fileStream);
        cout << "CTrapezoid height:" << mHeight << endl;
    }
    if (infoByte.isX) {
        mX = parseInt(fileStream);
        cout << "CTrapezoid X:" << mX << endl;
        _previousX = mX;
    } else {
        mX = _previousX;
    }
    if (infoByte.isY) {
        mY = parseInt(fileStream);
        cout << "CTrapezoid Y:" << mY << endl;
        _previousY = mY;
    } else {
        mY = _previousY;
    }
    if (infoByte.isRepetition) {
        mRepetition = parseRepetition(fileStream);
    }
}

const std::vector<KPoint> CTrapezoid::getPoints() {
    vector<KPoint> result;
    switch (mType) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
    {
        KPoint p1 = KPoint(mX, mY);
        result.push_back(p1);
        KPoint p2 = KPoint(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3 = KPoint(mX + mWidth - mHeight, mY + mHeight);
        result.push_back(p3);
        KPoint p4 = KPoint(mX + mHeight, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        break;
    case 12:
        break;
    case 13:
        break;
    case 14:
        break;
    case 15:
        break;
    case 16:
        break;
    case 17:
        break;
    case 18:
        break;
    case 19:
        break;
    case 20:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mHeight * 2, mY);
        result.push_back(p2);
        KPoint p3(mX + mHeight, mY + mHeight);
        result.push_back(p3);
        break;
    }
    case 21:
        break;
    case 22:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY + mWidth);
        result.push_back(p2);
        KPoint p3(mX, mY + mWidth * 2);
        result.push_back(p3);
        break;
    }
    case 23:
        break;
    case 24:
        break;
    case 25:
        break;
    default:
        break;
    }
    return result;
}

}
