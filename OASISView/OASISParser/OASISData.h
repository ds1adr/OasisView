#ifndef OASISDATA_H
#define OASISDATA_H

#include <string>

class OASISData
{
private:
    std::string mVersion;
    int mResolution;

    int parseMagicBytes(std::ifstream& fileStream);
    int parseStart(std::ifstream& fileStream);
public:
    OASISData();

    void parse(const std::string& filePath);
};

#endif // OASISDATA_H
