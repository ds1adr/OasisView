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
    OASISParser::byte_t isExplicit: 2;  // First byte is always 0 C
    bool isReferenceNumber: 1;          // N
    bool isX: 1;                        // X
    bool isY: 1;                        // Y
    bool isRepetition: 1;               // R
    bool isTextTypeNumber: 1;           // T
    bool isTextLayer: 1;                // L

};

class TextElement: public CellElement {
public:
    TextElement() = default;
    ~TextElement();
private:
    unsigned int mReference = 0;
    std::string mText;
    unsigned int mTextLayer;
    unsigned int mTextType;
    int mX = 0;
    int mY = 0;
    std::variant<OASISParser::Repetition, OASISParser::NSpaceRepetition, OASISParser::DiagonalRepetition, OASISParser::NDisplacementRepetition> mRepetition;

    void parse(std::ifstream& fileStream);
};

#endif // OASISCELLRECORDS_H
