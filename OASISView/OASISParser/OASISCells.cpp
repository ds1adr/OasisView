#include "OASISCells.h"
#include "OASISCellRecords.h"

#include <fstream>

namespace OASISParser {

OASISCell::OASISCell(const std::string& name) {
    mName = name;
}

// { CBLOCK | PAD | PROPERTY | XYRELATIVE | XYABSOLUTE | <element> }*
// <element> = { <geometry> | PLACEMENT | TEXT | XELEMENT }
// <geometry> = { RECTANGLE | POLYGON | PATH | TRAPZOID | CTRAPEZOID | CIRCLE | XGEOMETRY |
void OASISCell::parse(std::ifstream& fileStream) {
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
        CellElement* placementElement = new PlacementElement();
        placementElement->parse(fileStream);
        mCellElements.push_back(placementElement);
        break;
    }
    case 18: // PLACEMENT
        break;
    case 19: // TEXT
    {
        CellElement* textElement = new TextElement();
        textElement->parse(fileStream);
        mCellElements.push_back(textElement);
    }
        break;
    case 20: // RECTANGLE
    {
        CellElement* rectangleElement = new RectangleElement();
        rectangleElement->parse(fileStream);
        mCellElements.push_back(rectangleElement);
        break;
    }
    case 21: // POLYGON
        break;
    case 22: // PATH
        break;
    case 23: // TRAPEZOID
        break;
    case 24: // TRAPEZOID
        break;
    case 25: // TRAPEZOID
        break;
    case 26: // CTRAPEZOID
        break;
    case 27: // CIRCLE
        break;
    case 28: // PROPERTY
    {
        PropertyElement* propertyElement = new PropertyElement();
        propertyElement->parse(fileStream);
        mCellElements.push_back(propertyElement);
    }
        break;
    case 29: // Last PROPERTY
        break;
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