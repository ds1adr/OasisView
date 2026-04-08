#ifndef OASISELEMENTPARSER_H
#define OASISELEMENTPARSER_H

#include <string>

#include "OASISElement.h"

namespace OASISParser {

struct UIntTypeByte {
    byte_t value: 7;
    byte_t continuous: 1;
};

struct IntTypeByte {
    byte_t sign: 1;
    byte_t value: 6;
    byte_t continuous: 1;
};

struct Delta2TypeByte {
    byte_t direction: 2;
    byte_t value: 5;
    byte_t continuous: 1;
};

struct Delta3TypeByte {
    byte_t direction: 3;
    byte_t value: 4;
    byte_t continuous: 1;
};

struct DeltaGForm1Byte {
    byte_t type: 1;
    byte_t direction: 3;
    byte_t value: 3;
    byte_t continuous: 1;
};

struct DeltaGForm2FirstByte {
    byte_t type: 1;
    byte_t direction: 1;
    byte_t value: 5;
    byte_t continuous: 1;
};

struct DeltaGForm2SecondByte {
    byte_t direction: 1;
    byte_t value: 6;
    byte_t continuous: 1;
};

int parseInt(byte_t* mem, unsigned int& offset);
unsigned int parseUInt(byte_t* mem, unsigned int& offset);
unsigned int parseUInt(std::ifstream& fileStream);
double parseRealNumber(byte_t* mem, unsigned int& offset);
double parseRealNumber(std::ifstream& fileStream);

// Delta Type
// 1 delta = int
int parse1Delta(byte_t* mem, unsigned int& offset);
// 2 delta
Delta2 parse2Delta(byte_t* mem, unsigned int& offset);
// 3 delta
Delta3 parse3Delta(byte_t* mem, unsigned int& offset);
// g delta
Delta3 parseGDelta(byte_t* mem, unsigned int& offset);

extern byte_t lastRepetition;
// positive whole number (uint -> real)
double parseRealType0(byte_t* mem, unsigned int& offset);
double parseRealType0(std::ifstream& fileStream);
// negative whole number -(uint)
double parseRealType1(byte_t* mem, unsigned int& offset);
double parseRealType1(std::ifstream& fileStream);
// 1 / uint
double parseRealType2(byte_t* mem, unsigned int& offset);
double parseRealType2(std::ifstream& fileStream);
// -1 / uint
double parseRealType3(byte_t* mem, unsigned int& offset);
double parseRealType3(std::ifstream& fileStream);
// uint / uint
double parseRealType4(byte_t* mem, unsigned int& offset);
double parseRealType4(std::ifstream& fileStream);
// - uint / uint
double parseRealType5(byte_t* mem, unsigned int& offset);
double parseRealType5(std::ifstream& fileStream);
// Float
double parseRealType6(byte_t* mem, unsigned int& offset);
double parseRealType6(std::ifstream& fileStream);
// Double
double parseRealType7(byte_t* mem, unsigned int& offset);
double parseRealType7(std::ifstream& fileStream);
// A-String ASCII string space (0x20), 21 ~ 7E
const std::string parseAString(byte_t* mem, unsigned int& offset);
const std::string parseAString(std::ifstream& fileStream);
// B-String Binary String : any code
const std::string parseBString(byte_t* mem, unsigned int& offset);
const std::string parseBString(std::ifstream& fileStream);
// N-String Name String
const std::string parseNString(byte_t* mem, unsigned int& offset);
const std::string parseNString(std::ifstream& fileStream);

// g delta form 1
Delta3 parseGDeltaForm1(byte_t *mem, unsigned int &offset);
// g delta form 2
Delta3 parseGDeltaForm2(byte_t* mem, unsigned int &offset);

// Repetition
byte_t getRepetitionType(byte_t *mem, unsigned int &offset);
Repetition parseRepetitionType1(byte_t *mem, unsigned int &offset);
Repetition parseRepetitionType2(byte_t *mem, unsigned int &offset);
Repetition parseRepetitionType3(byte_t *mem, unsigned int &offset);
NSpaceRepetition parseRepetitionType4(byte_t *mem, unsigned int &offset);
NSpaceRepetition parseRepetitionType5(byte_t *mem, unsigned int &offset);
NSpaceRepetition parseRepetitionType6(byte_t *mem, unsigned int &offset);
NSpaceRepetition parseRepetitionType7(byte_t *mem, unsigned int &offset);
DiagonalRepetition parseRepetitionType8(byte_t *mem, unsigned int &offset);
DiagonalRepetition parseRepetitionType9(byte_t *mem, unsigned int &offset);
NDisplacementRepetition parseRepetitionType10(byte_t *mem, unsigned int &offset);
NDisplacementRepetition parseRepetitionType11(byte_t *mem, unsigned int &offset);

}

#endif // OASISELEMENTPARSER_H
