#ifndef OASISCELLRECORDS_H
#define OASISCELLRECORDS_H

#include <string>
#include "OASISElement.h"

namespace OASISParser {

class CellElement {
public:
    CellElement() = default;
    virtual ~CellElement() = default;

    virtual void parse(std::ifstream& fileStream) = 0;
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

    void parse(std::ifstream& fileStream);
private:
    unsigned int mReference = 0;
    std::string mText;
    unsigned int mTextLayer;
    unsigned int mTextType;
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

    void parse(std::ifstream& fileStream);
private:
    unsigned int mReference;
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
    bool isRefrence: 1; // N
    bool isExplicit: 1; // C
};

class Placement: public CellElement {
public:
    Placement() = default;
    ~Placement();

    void parse(std::ifstream& fileStream);
private:
    bool mIsFlip = false; // x-axis
    double mRotation = 0;
    double mMag; // Magnification
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> mRepetition;
    int mX = 0;
    int mY = 0;
    unsigned int mReference;
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

    void parse(std::ifstream& fileStream);
private:
    unsigned int mLayerNumber;
    unsigned int mDataType;
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> mRepetition;
    int mY = 0;
    int mX = 0;
    unsigned int mHeight;
    unsigned int mWidth;
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

class Trapzoid : public CellElement {
public:
    Trapzoid(unsigned int code);
    ~Trapzoid();

    void parse(std::ifstream& fileStream);
private:
    unsigned int mCode;  // 23: delta-a, delta-b, 24: delta-a, 25: delta-b

    unsigned int mLayerNumber;
    unsigned int mDataType;
    unsigned int mWidth;
    unsigned int mHeight;
    int mDeltaA = 0;
    int mDeltaB = 0;
    int mX = 0;
    int mY = 0;
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> mRepetition;
    Orientation mOrientation;
};

}

#endif // OASISCELLRECORDS_H
