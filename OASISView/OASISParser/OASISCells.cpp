#include "OASISCells.h"
#include "OASISCellRecords.h"

#include <fstream>
#include <unordered_set>

using namespace std;

namespace OASISParser {

Property* _lastProperty;

OASISCell::OASISCell(unsigned int reference): mReference(reference) {

}

OASISCell::OASISCell(const std::string& name): mName(name) {

}

// { CBLOCK | PAD | PROPERTY | XYRELATIVE | XYABSOLUTE | <element> }*
// <element> = { <geometry> | PLACEMENT | TEXT | XELEMENT }
// <geometry> = { RECTANGLE | POLYGON | PATH | TRAPZOID | CTRAPEZOID | CIRCLE | XGEOMETRY |
void OASISCell::parse(ifstream& fileStream, unordered_set<unsigned>& layerSet) {
    bool isQuit = false;
    while (isQuit == false) {
    char type;
    fileStream.read(&type, sizeof(type));

    switch (type) {
    case 0: // PAD
        break;
    case 15: // XYABSOLUTE
        break;
    case 16: // XYRELATIVE
        break;
    case 17: // PLACEMENT
    {
        CellElement* placement = new Placement();
        placement->parse(fileStream, layerSet);
        mCellElements.push_back(placement);
        break;
    }
    case 18: // PLACEMENT
        break;
    case 19: // TEXT
    {
        CellElement* text = new Text();
        text->parse(fileStream, layerSet);
        mCellElements.push_back(text);
    }
        break;
    case 20: // RECTANGLE
    {
        CellElement* rectangle = new Rectangle();
        rectangle->parse(fileStream, layerSet);
        mCellElements.push_back(rectangle);
        break;
    }
    case 21: // POLYGON
        break;
    case 22: // PATH
        break;
    case 23: // TRAPEZOID
    case 24:
    case 25:
    {
        CellElement* trapezoid = new Trapezoid(type);
        trapezoid->parse(fileStream, layerSet);
        mCellElements.push_back(trapezoid);
        break;
    }
    case 26: // CTRAPEZOID
    {
        CellElement* cTrapezoid = new CTrapezoid();
        cTrapezoid->parse(fileStream, layerSet);
        mCellElements.push_back(cTrapezoid);
        break;
    }
    case 27: // CIRCLE
        break;
    case 28: // PROPERTY
    {
        Property* property = new Property();
        property->parse(fileStream, layerSet);
        _lastProperty = property;
        mCellElements.push_back(property);
    }
        break;
    case 29: // Last PROPERTY
    {
        Property* property = new Property();
        property->setValues(_lastProperty);
        break;
    }
    case 32: // XELEMENT
        break;
    case 33: // XGEOMETRY
        break;
    case 34: // CBLOCK
        break;
    default:
        isQuit = true;
        fileStream.seekg(-1, std::ios::cur);
        break;
    }

    }
}

OASISCell::~OASISCell() {
    for (CellElement* element : mCellElements) {
        delete element;
    }
    mCellElements.clear();
}

OASISCellRef::OASISCellRef() {

}

}