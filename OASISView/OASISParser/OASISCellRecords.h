#ifndef OASISCELLRECORDS_H
#define OASISCELLRECORDS_H

#include <string>
#include <unordered_set>
#include <variant>

#include "OASISElement.h"

namespace OASISParser {

struct BoundingBox;
class OASISData;

class CellElement {
public:
    CellElement() = default;
    virtual ~CellElement() = default;

    virtual void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet) = 0;
    virtual std::string elementName() = 0;
};

// 0CNXYRTL
struct TextInfoByte {
    bool isTextLayer: 1;                // L
    bool isTextTypeNumber: 1;           // T
    bool isRepetition: 1;               // R
    bool isY: 1;                        // Y
    bool isX: 1;                        // X
    bool isReferenceNumber: 1;          // N
    bool isExplicit: 1;  // First byte is always 0 C
};

class Text: public CellElement {
public:
    Text() = default;
    ~Text();

    void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet);
    std::string elementName() { return "Text"; }
private:
    unsigned mReference = 0;
    std::string mText;
    unsigned mTextLayer;
    unsigned mTextType;
    int mX = 0;
    int mY = 0;
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> mRepetition;
};

struct PropertyInfoByte {
    bool isStandard: 1;     // S
    bool isReference: 1;    // N
    bool isExplicit: 1;     // C
    bool isPropertyValue: 1;    // V
    int numberOfValue: 4;   // UUUU
};

class Property: public CellElement {
public:
    Property() = default;
    ~Property();

    std::string elementName() { return "Property"; }

    void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet);
    void setValues(Property* origin);

    unsigned getReference() { return mReference; }
    bool getStandard() { return mStandard; }
    std::string getPropName() { return mPropNameString; }
    unsigned getPropValueCount() { return mPropValueCount; }
    std::vector<std::variant<float, double, unsigned int, int, std::string>> getProperty() { return mProperty; }
private:
    unsigned mReference;
    bool mStandard;
    std::string mPropNameString;
    unsigned int mPropValueCount;
    std::vector<std::variant<float, double, unsigned int, int, std::string>> mProperty;
};

struct PlacementInfoByte {
    bool isFlip: 1;     // F
    char rotation: 2;   // AA
    bool isRepetition: 1; // R
    bool isY: 1;        // Y
    bool isX: 1;        // X
    bool isReference: 1; // N
    bool isExplicit: 1; // C
};

// With Maginfication
struct MPlacementInfoByte {
    bool isFlip: 1;     // F
    bool isAngle: 1;    // A
    bool isMag: 1;      // M
    bool isRepetition: 1; // R
    bool isY: 1;        // Y
    bool isX: 1;        // X
    bool isReference: 1;// N
    bool isExplicit: 1; // C
};

class Placement: public CellElement {
public:
    Placement(unsigned code): mCode(code) {};
    ~Placement();

    std::string elementName() { return "Placement"; }

    void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet);
    const unsigned getReference() { return mReference; }
    const std::string& getCellName() { return mCellName; }

    BoundingBox calculateBoundingBox(OASISData& oasisData);

    const std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition>& getRepetition() { return mRepetition; }
    int getX() { return mX; }
    int getY() { return mY; }
private:
    unsigned mCode;
    bool mIsFlip = false; // x-axis
    double mRotation = 0;
    double mMag = 1.0;
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> mRepetition;
    int mX = 0;
    int mY = 0;
    unsigned mReference = 0;
    std::string mCellName;
};

struct RectangleInfoByte {
    bool isLayerNumber: 1;      // L
    bool isDataType: 1;         // D
    bool isRepetation: 1;       // R
    bool isY: 1;                // Y
    bool isX: 1;                // X
    bool isHeight: 1;           // H
    bool isWidth: 1;            // W
    bool isSquare: 1;           // S
};

class Rectangle: public CellElement {
public:
    Rectangle() = default;
    ~Rectangle();

    std::string elementName() { return "Rectangle"; }

    void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet);
    int getMinX() { return mX; }
    int getMinY() { return mY; }
    int getMaxX() { return mX + mWidth; }
    int getMaxY() { return mY + mHeight; }
    unsigned getWidth() { return mWidth; }
    unsigned getHeight() { return mHeight; }
    const std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition>& getRepetition() { return mRepetition; }
private:
    unsigned mLayerNumber;
    unsigned mDataType;
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> mRepetition;
    int mY = 0;
    int mX = 0;
    unsigned mHeight;
    unsigned mWidth;
};

struct TrapInfoByte {
    bool isLayerNumber: 1;      // L
    bool isDataType: 1;         // D
    bool isRepetition: 1;       // R
    bool isY: 1;                // Y
    bool isX: 1;                // X
    bool isHeight: 1;           // H
    bool isWidth:1;             // W
    bool isVertical: 1;         // O
};

enum class Orientation {
    Vertical, Horizontal
};

struct KPoint {
    int x, y;

    KPoint(int _x, int _y) { x = _x; y = _y;};
};

struct KDelta {
    int dx, dy;

    KDelta(int _dx, int _dy) { dx = _dx; dy = _dy; }
};

class Trapezoid : public CellElement {
public:
    Trapezoid(unsigned int code);
    ~Trapezoid();

    std::string elementName() { return "Trapezoid"; }

    void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet);
    int getMinX() { return mX; }
    int getMinY() { return mY; }
    int getMaxX() { return mX + mWidth; }
    int getMaxY() { return mX + mHeight; }

    const std::vector<KPoint> getInitialPoints();
    const std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition>& getRepetition() { return mRepetition; }
private:
    unsigned int mCode;  // 23: delta-a, delta-b, 24: delta-a, 25: delta-b

    unsigned mLayerNumber;
    unsigned mDataType;
    unsigned mWidth;
    unsigned mHeight;
    int mDeltaA = 0;
    int mDeltaB = 0;
    int mX = 0;
    int mY = 0;
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> mRepetition;
    Orientation mOrientation;
};

struct CTrapInfoByte {
    bool isLayerNumber: 1;      // L
    bool isDataType: 1;         // D
    bool isRepetition: 1;       // R
    bool isY: 1;                // Y
    bool isX: 1;                // X
    bool isHeight: 1;           // H
    bool isWidth: 1;            // W
    bool isType: 1;             // T
};

class CTrapezoid : public CellElement {
public:
    CTrapezoid() = default;
    ~CTrapezoid();

    std::string elementName() { return "CTrapezoid"; }

    void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet);
    unsigned getType() { return mType; }
    const std::vector<KPoint> getInitialPoints();

    int getMinX() { return mX; }
    int getMinY() { return mY; }
    int getMaxX();
    int getMaxY();

    const std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition>& getRepetition() { return mRepetition; }
private:
    unsigned mLayerNumber;
    unsigned mDataType;
    unsigned mType;
    unsigned mWidth;
    unsigned mHeight;
    int mX = 0;
    int mY = 0;
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> mRepetition;
};

struct PolygonInfoByte {
    bool isLayerNumber: 1;      // L
    bool isDataType: 1;         // D
    bool isRepetition: 1;       // R
    bool isY: 1;                // Y
    bool isX: 1;                // X
    bool isPointList: 1;        // P
};

class Polygon : public CellElement {
public:
    Polygon() = default;
    ~Polygon();

    std::string elementName() { return "Polygon"; }

    void parse(std::ifstream& fileStream, std::unordered_set<unsigned>& layerSet);

    BoundingBox calculateBoundingBox(OASISData& oasisData);

private:
    unsigned mLayerNumber;
    unsigned mDataType;
    unsigned mType = 0;
    int mX = 0;
    int mY = 0;
    std::vector<KPoint> mPoints;
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> mRepetition;
};

}



#endif // OASISCELLRECORDS_H
