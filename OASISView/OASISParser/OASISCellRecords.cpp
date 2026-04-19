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
unsigned _previousPathHalfWidth = 0;
vector<KDelta> _previousPolygonDeltas;
vector<KDelta> _previousPathDeltas;

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
        if (mIsFlip) {
            int temp = subBBox.minY;
            subBBox.minY = -subBBox.maxY;
            subBBox.maxY = -temp;
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

const vector<KPoint> Trapezoid::getInitialPoints() {
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

int CTrapezoid::getMaxX() {
    switch (mType) {
    case 20:
    case 21:
        return mX + 2 * mHeight;
    default:
        return mX + mWidth;
    }
}

int CTrapezoid::getMaxY() {
    switch (mType) {
    case 16:
    case 17:
    case 18:
    case 19:
        return mY + mWidth;
    case 22:
    case 23:
        return mY + 2 * mWidth;
    default:
        return mY + mHeight;
    }
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

const std::vector<KPoint> CTrapezoid::getInitialPoints() {
    vector<KPoint> result;
    switch (mType) {
    case 0:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth - mHeight, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 1:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth - mHeight, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 2:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX + mHeight, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 3:
    {
        KPoint p1(mX, mY + mHeight);
        result.push_back(p1);
        KPoint p2(mX + mHeight, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY);
        result.push_back(p3);
        KPoint p4(mX + mWidth, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 4:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth - mHeight, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX + mHeight, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 5:
    {
        KPoint p1(mX, mY + mHeight);
        result.push_back(p1);
        KPoint p2(mX + mHeight, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth - mHeight, mY);
        result.push_back(p3);
        KPoint p4(mX + mWidth, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 6:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth - mHeight, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX + mHeight, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 7:
    {
        KPoint p1(mX, mY + mHeight);
        result.push_back(p1);
        KPoint p2(mX + mHeight, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY);
        result.push_back(p3);
        KPoint p4(mX + mWidth - mHeight, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 8:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight - mWidth);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 9:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX, mY + mWidth);
        result.push_back(p4);
        break;
    }
    case 10:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY + mWidth);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 11:
    {
        KPoint p1(mX, mY + mWidth);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 12:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY + mWidth);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight - mWidth);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 13:
    {
        KPoint p1(mX, mY + mWidth);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight - mWidth);
        result.push_back(p4);
        break;
    }
    case 14:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY + mWidth);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight - mWidth);
        result.push_back(p4);
        break;
    }
    case 15:
    {
        KPoint p1(mX, mY + mWidth);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight - mWidth);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight);
        result.push_back(p4);
        break;
    }
    case 16:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX, mY + mWidth);
        result.push_back(p3);
        break;
    }
    case 17:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY + mWidth);
        result.push_back(p2);
        KPoint p3(mX, mY + mWidth);
        result.push_back(p3);
        break;
    }
    case 18:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mWidth);
        result.push_back(p3);
        break;
    }
    case 19:
    {
        KPoint p1(mX, mY + mWidth);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mWidth);
        result.push_back(p3);
        break;
    }
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
    {
        KPoint p1(mX, mY + mHeight);
        result.push_back(p1);
        KPoint p2(mX + mHeight, mY);
        result.push_back(p2);
        KPoint p3(mX + 2* mHeight, mY + mHeight);
        result.push_back(p3);
        break;
    }
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
    {
        KPoint p1(mX, mY + mWidth);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mWidth * 2);
        result.push_back(p3);
        break;
    }
    case 24:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mHeight);
        result.push_back(p3);
        KPoint p4(mX, mY + mHeight);
        break;
    }
    case 25:
    {
        KPoint p1(mX, mY);
        result.push_back(p1);
        KPoint p2(mX + mWidth, mY);
        result.push_back(p2);
        KPoint p3(mX + mWidth, mY + mWidth);
        result.push_back(p3);
        KPoint p4(mX, mY + mWidth);
        break;
    }
    default:
        break;
    }
    return result;
}

Polygon::~Polygon() {

}

void Polygon::parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet) {
    PolygonInfoByte infoByte;  // TWHXYRDL

    fileStream.read((char*)&infoByte, sizeof(char));

    cout << "Polygon" << endl;
    if (infoByte.isLayerNumber) {
        mLayerNumber = parseUInt(fileStream);
        layerSet.insert(mLayerNumber);
        cout << "Polygon Layer:" << mLayerNumber << endl;
    }
    if (infoByte.isDataType) {
        mDataType = parseUInt(fileStream);
    }
    unsigned type;
    vector<KDelta> deltas;

    if (infoByte.isPointList) {
        deltas = parsePointLists(fileStream, type);
        _previousPolygonDeltas = deltas;
    } else {
        deltas = _previousPolygonDeltas;
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
    // Make points
    cout << "mX:" << mX << "," << mY << endl;
    KPoint last = KPoint(mX, mY);
    mPoints.push_back(last);
    for (KDelta delta : deltas) {
        last = KPoint(last.x + delta.dx, last.y + delta.dy);
        cout << "Point:" << last.x << "," << last.y << endl;
        mPoints.push_back(last);
    }
    if (type == 0 || type ==1) {
        if (deltas.back().dx == 0) {
            mPoints.push_back(KPoint(mPoints[0].x, mPoints.back().y));
        } else {
            mPoints.push_back(KPoint(mPoints.back().x, mPoints[0].y));
        }
        mPoints.push_back(KPoint(mPoints[0].x, mPoints[0].y));
    } else {
        mPoints.push_back(KPoint(mPoints[0].x, mPoints[0].y));
    }
    if (infoByte.isRepetition) {
        mRepetition = parseRepetition(fileStream);
    }
}

BoundingBox Polygon::calculateBoundingBox(OASISData& oasisData) {
    BoundingBox bBox;
    for (KPoint p : mPoints) {
        bBox.minX = min(p.x, bBox.minX);
        bBox.minY = min(p.y, bBox.minY);
        bBox.maxX = max(p.x, bBox.maxX);
        bBox.maxY = max(p.y, bBox.maxY);
    }
    return bBox;
}

Path::~Path() {

}

void Path::parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet) {
    PathInfoByte infoByte;  // EWPXYRDL
    ExtensionByte extByte;

    fileStream.read((char*)&infoByte, sizeof(char));

    cout << "Polygon" << endl;
    if (infoByte.isLayerNumber) {
        mLayerNumber = parseUInt(fileStream);
        layerSet.insert(mLayerNumber);
        cout << "Polygon Layer:" << mLayerNumber << endl;
    }
    if (infoByte.isDataType) {
        mDataType = parseUInt(fileStream);
    }
    if (infoByte.isHalfWidth) {
        mHalfWidth = parseUInt(fileStream);
        _previousPathHalfWidth = mHalfWidth;
    } else {
        mHalfWidth = _previousPathHalfWidth;
    }
    if (infoByte.isExtensionScheme) {
        fileStream.read((char*)&extByte, sizeof(char));
        cout << "Ext" << " SS:" << extByte.startExtension << " End:" << extByte.endExension << " Value:" << extByte.value << endl;
        if (extByte.startExtension == 3) {
            mStartExt = parseInt(fileStream);
        }
        if (extByte.endExension == 3) {
            mEndExt = parseInt(fileStream);
        }
    }
    vector<KDelta> deltas;
    if (infoByte.isPointLists) {
        unsigned type;
        deltas = parsePointLists(fileStream, type);
        _previousPathDeltas = deltas;
    } else {
        deltas = _previousPathDeltas;
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
