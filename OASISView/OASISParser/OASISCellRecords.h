#ifndef OASISCELLRECORDS_H
#define OASISCELLRECORDS_H

#include <string>
#include "OASISElement.h"

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

class TextElement: public CellElement {
public:
    TextElement() = default;
    ~TextElement();

    void parse(std::ifstream& fileStream);
private:
    unsigned int mReference = 0;
    std::string mText;
    unsigned int mTextLayer;
    unsigned int mTextType;
    int mX = 0;
    int mY = 0;
    std::variant<OASISParser::Repetition, OASISParser::NSpaceRepetition, OASISParser::DiagonalRepetition, OASISParser::NDisplacementRepetition> mRepetition;
};

struct PropertyInfoByte {
    bool isStandard: 1;     // S
    bool isReference: 1;    // N
    bool isExplicit: 1;     // C
    bool isPropertyValue: 1;    // V
    int numberOfValue: 4;   // UUUU
};

class PropertyElement: public CellElement {
public:
    PropertyElement() = default;
    ~PropertyElement();

    void parse(std::ifstream& fileStream);
private:
    unsigned int mReference;
    bool mStandard;
    std::string mPropNameString;
    unsigned int mPropValueCount;
    std::vector<std::variant<float, double, unsigned int, int, std::string>> mProperty;
};

#endif // OASISCELLRECORDS_H
