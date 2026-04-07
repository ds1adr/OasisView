#ifndef OASISDATA_H
#define OASISDATA_H

#include <string>

class OASISData
{
private:
    std::string mVersion;
    double mUnit;
    unsigned int mOffsetFlag; // 0 : Table offset is in the Start, 1: Table offset is in the End

    int parseMagicBytes(std::ifstream& fileStream);
    int parseStart(std::ifstream& fileStream);
public:
    OASISData();

    void parse(const std::string& filePath);
};

#endif // OASISDATA_H
