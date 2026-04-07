#include <cstring>
#include <fstream>

#include "OASISElementParser.h"
#include "OASISParserException.h"

namespace OASISParser {

const int validBitSize = 7;
const int validBitSizeForSigned = 6;
byte_t lastRepetition = 0;

int parseInt(byte_t* mem, unsigned int& offset) {
    unsigned int result = 0;
    int totalBitSize = 0;
    int sign = 1;
    bool isContinuous = false;

    // TODO: need to check bitSize is larger than int size
    do {
        int iValue = 0;

        if (totalBitSize == 0) {
            IntTypeByte *b = (IntTypeByte*)&mem[offset];

            sign = (b->sign == 1) ? -1 : 1;
            isContinuous = (b->continuous == 1);
            iValue = b->value;

            iValue = iValue << totalBitSize;
            result = result | iValue;
            totalBitSize += validBitSizeForSigned;
        } else {
            UIntTypeByte *b = (UIntTypeByte*)&mem[offset];

            isContinuous = (b->continuous == 1);
            iValue = b->value;

            iValue = iValue << totalBitSize;
            result = result | iValue;
            totalBitSize += validBitSize;
        }

        offset++;
    } while (isContinuous);

    return (result * sign);
}

unsigned int parseUInt(byte_t* mem, unsigned int& offset) {
    bool isContinuous = false;
    unsigned int result = 0;
    int totalBitSize = 0;

    // TODO: need to check bitSize is larger than int size
    do {
        UIntTypeByte *b = (UIntTypeByte*)&mem[offset];

        int iValue = b->value;
        iValue = iValue << totalBitSize;
        isContinuous = (b->continuous == 1);

        result = result | iValue;
        totalBitSize += validBitSize;
        offset++;
    } while (isContinuous);

    return result;
}

unsigned int parseUInt(std::ifstream& fileStream) {
    bool isContinuous = false;
    unsigned int result = 0;
    int totalBitSize = 0;
    char buffer;

    // TODO: need to check bitSize is larger than int size
    do {
        fileStream.read(&buffer, sizeof(buffer));
        UIntTypeByte *b = (UIntTypeByte*)&buffer;

        int iValue = b->value;
        iValue = iValue << totalBitSize;
        isContinuous = (b->continuous == 1);

        result = result | iValue;
        totalBitSize += validBitSize;
    } while (isContinuous);

    return result;
}

double parseRealNumber(byte_t* mem, unsigned int& offset) {
    byte_t type = mem[offset];
    offset++;

    switch (type) {
    case 0:
        return parseRealType0(mem, offset);
    case 1:
        return parseRealType1(mem, offset);
    case 2:
        return parseRealType2(mem, offset);
    case 3:
        return parseRealType3(mem, offset);
    case 4:
        return parseRealType4(mem, offset);
    case 5:
        return parseRealType5(mem, offset);
    case 6:
        return parseRealType6(mem, offset);
    case 7:
        return parseRealType7(mem, offset);
    default:
        return 0.0;
    }
}

double parseRealNumber(std::ifstream& fileStream) {
    char type;
    fileStream.read(&type, sizeof(type));

    switch (type) {
    case 0:
        return parseRealType0(fileStream);
    case 1:
        return parseRealType1(fileStream);
    case 2:
        return parseRealType2(fileStream);
    case 3:
        return parseRealType3(fileStream);
    case 4:
        return parseRealType4(fileStream);
    case 5:
        return parseRealType5(fileStream);
    case 6:
        return parseRealType6(fileStream);
    case 7:
        return parseRealType7(fileStream);
    default:
        return 0.0;
    }
}

double parseRealType0(byte_t* mem, unsigned int& offset) {
    return (double)parseUInt(mem, offset);
}

double parseRealType0(std::ifstream& fileStream) {
    return (double)parseUInt(fileStream);
}

double parseRealType1(byte_t* mem, unsigned int& offset) {
    return -(double)parseUInt(mem, offset);
}

double parseRealType1(std::ifstream& fileStream) {
    return -(double)parseUInt(fileStream);
}

double parseRealType2(byte_t* mem, unsigned int& offset) {
    return 1.0 / (double)parseUInt(mem, offset);
}

double parseRealType2(std::ifstream& fileStream) {
    return 1.0 / (double)parseUInt(fileStream);
}

double parseRealType3(byte_t* mem, unsigned int& offset) {
    return -1.0 / (double)parseUInt(mem, offset);
}

double parseRealType3(std::ifstream& fileStream) {
    return -1.0 / (double)parseUInt(fileStream);
}

double parseRealType4(byte_t* mem, unsigned int& offset) {
    return (double)parseUInt(mem, offset) / (double)parseUInt(mem, offset);
}

double parseRealType4(std::ifstream& fileStream) {
    return (double)parseUInt(fileStream) / (double)parseUInt(fileStream);
}

double parseRealType5(byte_t* mem, unsigned int& offset) {
    return -(double)parseUInt(mem, offset) / (double)parseUInt(mem, offset);
}

double parseRealType5(std::ifstream& fileStream) {
    return -(double)parseUInt(fileStream);
}

double parseRealType6(byte_t* mem, unsigned int& offset) {
    float value;
    memcpy(&value, (mem + offset), sizeof(float));
    offset += sizeof(float);
    return (double)value;
}

double parseRealType6(std::ifstream& fileStream) {
    float value;
    fileStream.read((char*)&value, sizeof(float));
    return (double)value;
}

double parseRealType7(byte_t* mem, unsigned int& offset) {
    double value;
    memcpy(&value, (mem + offset), sizeof(double));
    offset += sizeof(double);
    return value;
}

double parseRealType7(std::ifstream& fileStream) {
    double value;
    fileStream.read((char*)&value, sizeof(double));
    return value;
}

const std::string parseAString(byte_t* mem, unsigned int& offset) {
    unsigned int length = parseUInt(mem, offset);
    int initOffset = offset;
    offset += length;

    for (unsigned int i = 0; i < length; i++) {
        char value = *((char *)(mem + initOffset + i));
        if ( value < 0x20 || value > 0x7e ) {
            throw InvalidString();
        }
    }

    return std::string((char *)(mem + initOffset), length);
}

const std::string parseAString(std::ifstream& fileStream) {
    unsigned int length = parseUInt(fileStream);
    char* buffer = new char[length];

    fileStream.read(buffer, length);

    std::string result = std::string((char *)(buffer), length);
    delete [] buffer;

    return result;
}

const std::string parseBString(byte_t* mem, unsigned int& offset) {
    unsigned int length = parseUInt(mem, offset);
    int initOffset = offset;
    offset += length;

    return std::string((char *)(mem + initOffset), length);
}

// N-String Name String
const std::string parseNString(byte_t* mem, unsigned int& offset) {
    unsigned int length = parseUInt(mem, offset);

    if (length == 0) {
        throw InvalidString();
    }

    int initOffset = offset;
    offset += length;

    for (unsigned int i = 0; i < length; i++) {
        char value = *((char *)(mem + initOffset + i));
        if ( value < 0x21 || value > 0x7e ) {
            throw InvalidString();
        }
    }

    return std::string((char *)(mem + initOffset), length);
}

// Delta type
int parse1Delta(byte_t* mem, unsigned int& offset) {
    return parseInt(mem, offset);
}

Delta2 parse2Delta(byte_t* mem, unsigned int& offset) {
    bool isContinuous = false;
    unsigned int result = 0;
    int totalBitSize = 0;
    unsigned int value = 0;
    Delta2 delta2 = Delta2();

    // TODO: need to check bitSize is larger than int size
    do {
        if (totalBitSize == 0) {
            Delta2TypeByte *b = (Delta2TypeByte*)&mem[offset];

            delta2.direction = (Direction)b->direction;
            isContinuous = (b->continuous == 1);
            value = b->value;

            value = value << totalBitSize;
            result = result | value;
            totalBitSize += 5;
        } else {

            UIntTypeByte *b = (UIntTypeByte*)&mem[offset];

            value = b->value;
            value = value << totalBitSize;
            isContinuous = (b->continuous == 1);

            result = result | value;
            totalBitSize += validBitSize;
        }
        offset++;
    } while (isContinuous);
    delta2.value = result;

    return delta2;
}

Delta3 parse3Delta(byte_t* mem, unsigned int& offset) {
    bool isContinuous = false;
    unsigned int result = 0;
    int totalBitSize = 0;
    unsigned int value = 0;
    int direction = 0;
    Delta3 delta3 = Delta3();

    do {
        if (totalBitSize == 0) {
            Delta3TypeByte *b = (Delta3TypeByte*)&mem[offset];

            direction = b->direction;
            isContinuous = (b->continuous == 1);
            value = b->value;

            value = value << totalBitSize;
            result = result | value;
            totalBitSize += 4;
        } else {

            UIntTypeByte *b = (UIntTypeByte*)&mem[offset];

            value = b->value;
            value = value << totalBitSize;
            isContinuous = (b->continuous == 1);

            result = result | value;
            totalBitSize += validBitSize;
        }
        offset++;
    } while (isContinuous);

    switch (direction) {
    case 0: // east
        delta3.dx = result;
        delta3.dy = 0;
        break;
    case 1: // north
        delta3.dx = 0;
        delta3.dy = result;
        break;
    case 2: // west
        delta3.dx = -result;
        delta3.dy = 0;
        break;
    case 3: // south
        delta3.dx = 0;
        delta3.dy = -result;
        break;
    case 4: // northeast
        delta3.dx = result;
        delta3.dy = result;
        break;
    case 5: // northwest
        delta3.dx = -result;
        delta3.dy = result;
        break;
    case 6: // southwest
        delta3.dx = -result;
        delta3.dy = -result;
        break;
    case 7: // southeast
        delta3.dx = result;
        delta3.dy = -result;
        break;
    default:
        break;
    }
    return delta3;
}

Delta3 parseGDelta(byte_t* mem, unsigned int& offset) {
    byte_t temp = mem[offset];
    if (temp & 0x01) { // type 2
        return parseGDeltaForm2(mem, offset);
    } else { // type 1
        return parseGDeltaForm1(mem, offset);
    }
}

// g delta form 1
Delta3 parseGDeltaForm1(byte_t *mem, unsigned int &offset) {
    bool isContinuous = false;
    unsigned int result = 0;
    int totalBitSize = 0;
    unsigned int value = 0;
    int direction = 0;
    Delta3 delta3 = Delta3();

    do {
        if (totalBitSize == 0) {
            DeltaGForm1Byte *b = (DeltaGForm1Byte*)&mem[offset];

            direction = b->direction;
            isContinuous = (b->continuous == 1);
            value = b->value;

            value = value << totalBitSize;
            result = result | value;
            totalBitSize += 3;
        } else {

            UIntTypeByte *b = (UIntTypeByte*)&mem[offset];

            value = b->value;
            value = value << totalBitSize;
            isContinuous = (b->continuous == 1);

            result = result | value;
            totalBitSize += validBitSize;
        }
        offset++;
    } while (isContinuous);

    switch (direction) {
    case 0: // east
        delta3.dx = result;
        delta3.dy = 0;
        break;
    case 1: // north
        delta3.dx = 0;
        delta3.dy = result;
        break;
    case 2: // west
        delta3.dx = -result;
        delta3.dy = 0;
        break;
    case 3: // south
        delta3.dx = 0;
        delta3.dy = -result;
        break;
    case 4: // northeast
        delta3.dx = result;
        delta3.dy = result;
        break;
    case 5: // northwest
        delta3.dx = -result;
        delta3.dy = result;
        break;
    case 6: // southwest
        delta3.dx = -result;
        delta3.dy = -result;
        break;
    case 7: // southeast
        delta3.dx = result;
        delta3.dy = -result;
        break;
    default:
        break;
    }
    return delta3;
}

// g delta form 2
Delta3 parseGDeltaForm2(byte_t* mem, unsigned int &offset) {
    bool isContinuous = false;
    unsigned int dx = 0;
    unsigned int dy = 0;
    unsigned int temp = 0;
    int totalBitSize = 0;
    int signX = 0;
    int signY = 0;

    do {
        if (totalBitSize == 0) {
            DeltaGForm2FirstByte *b = (DeltaGForm2FirstByte*)&mem[offset];

            signX = (b->direction == 0) ? 1 : -1;
            isContinuous = (b->continuous == 1);
            temp = b->value;

            temp = temp << totalBitSize;
            dx = dx | temp;
            totalBitSize += 5;
        } else {

            UIntTypeByte *b = (UIntTypeByte*)&mem[offset];

            temp = b->value;
            temp = temp << totalBitSize;
            isContinuous = (b->continuous == 1);

            dx = dx | temp;
            totalBitSize += validBitSize;
        }
        offset++;
    } while (isContinuous);

    totalBitSize = 0;
    do {
        if (totalBitSize == 0) {
            DeltaGForm2SecondByte *b = (DeltaGForm2SecondByte*)&mem[offset];

            signY = (b->direction == 0) ? 1 : -1;
            isContinuous = (b->continuous == 1);
            temp = b->value;

            temp = temp << totalBitSize;
            dy = dy | temp;
            totalBitSize += 6;
        } else {

            UIntTypeByte *b = (UIntTypeByte*)&mem[offset];

            temp = b->value;
            temp = temp << totalBitSize;
            isContinuous = (b->continuous == 1);

            dy = dy | temp;
            totalBitSize += validBitSize;
        }
        offset++;
    } while (isContinuous);

    return Delta3(signX * dx, signY * dy);
}

byte_t getRepetitionType(byte_t *mem, unsigned int &offset) {
    byte_t type = mem[offset++];
    if (type == 0) {
        return lastRepetition;
    } else {
        lastRepetition = type;
    }
    return type;
}

Repetition parseRepetitionType1(byte_t *mem, unsigned int &offset) {
    unsigned xdim = parseUInt(mem, offset);
    unsigned ydim = parseUInt(mem, offset);
    unsigned dx = parseUInt(mem, offset);
    unsigned dy = parseUInt(mem, offset);

    return Repetition(xdim, ydim, dx, dy);
}

Repetition parseRepetitionType2(byte_t *mem, unsigned int &offset) {
    unsigned xdim = parseUInt(mem, offset);
    unsigned dx = parseUInt(mem, offset);

    return Repetition(xdim, 0, dx, 0);
}

Repetition parseRepetitionType3(byte_t *mem, unsigned int &offset) {
    unsigned ydim = parseUInt(mem, offset);
    unsigned dy = parseUInt(mem, offset);

    return Repetition(0, ydim, 0, dy);
}

NSpaceRepetition parseRepetitionType4(byte_t *mem, unsigned int &offset) {
    NSpaceRepetition result;

    unsigned xdim = parseUInt(mem, offset);
    result.nx = xdim;
    result.ny = 0;
    result.grid = 1;

    for (unsigned i = 0; i < xdim - 1; i++) {
        unsigned dx = parseUInt(mem, offset);
        result.v_dx.push_back(dx);
    }

    return result;
}

NSpaceRepetition parseRepetitionType5(byte_t *mem, unsigned int &offset) {
    NSpaceRepetition result;

    unsigned xdim = parseUInt(mem, offset);
    unsigned grid = parseUInt(mem, offset);
    result.nx = xdim;
    result.ny = 0;
    result.grid = grid;

    for (unsigned i = 0; i < xdim - 1; i++) {
        unsigned dx = parseUInt(mem, offset);
        result.v_dx.push_back(dx);
    }

    return result;
}

NSpaceRepetition parseRepetitionType6(byte_t *mem, unsigned int &offset) {
    NSpaceRepetition result;

    unsigned ydim = parseUInt(mem, offset);
    result.ny = ydim;
    result.nx = 0;
    result.grid = 1;

    for (unsigned i = 0; i < ydim - 1; i++) {
        unsigned dy = parseUInt(mem, offset);
        result.v_dy.push_back(dy);
    }

    return result;
}

NSpaceRepetition parseRepetitionType7(byte_t *mem, unsigned int &offset) {
    NSpaceRepetition result;

    unsigned ydim = parseUInt(mem, offset);
    unsigned grid = parseUInt(mem, offset);
    result.nx = 0;
    result.ny = ydim;
    result.grid = grid;

    for (unsigned i = 0; i < ydim - 1; i++) {
        unsigned dy = parseUInt(mem, offset);
        result.v_dy.push_back(dy);
    }

    return result;
}

DiagonalRepetition parseRepetitionType8(byte_t *mem, unsigned int &offset) {
    DiagonalRepetition result;

    result.nDim = parseUInt(mem, offset);
    result.mDim = parseUInt(mem, offset);

    result.nDisplacement = parseGDelta(mem, offset);
    result.mDisplacement = parseGDelta(mem, offset);

    return result;
}

DiagonalRepetition parseRepetitionType9(byte_t *mem, unsigned int &offset) {
    DiagonalRepetition result;

    result.nDim = parseUInt(mem, offset);
    result.nDisplacement = parseGDelta(mem, offset);
    result.mDim = 0;

    return result;
}

NDisplacementRepetition parseRepetitionType10(byte_t *mem, unsigned int &offset) {
    NDisplacementRepetition result;

    result.n = parseUInt(mem, offset);
    result.grid = 1;

    for (unsigned i = 0; i < result.n - 1; i++) {
        Delta3 displacement = parseGDelta(mem, offset);
        result.v_d.push_back(displacement);
    }
    return result;
}

NDisplacementRepetition parseRepetitionType11(byte_t *mem, unsigned int &offset) {
    NDisplacementRepetition result;

    result.n = parseUInt(mem, offset);
    result.grid = parseUInt(mem, offset);

    for (unsigned i = 0; i < result.n - 1; i++) {
        Delta3 displacement = parseGDelta(mem, offset);
        result.v_d.push_back(displacement);
    }

    return result;
}

}
