#include "OASISCells.h"
#include "OASISCellRecords.h"

#include <fstream>
#include <iostream>
#include <unordered_set>

using namespace std;

namespace OASISParser {

Property* _lastProperty;

OASISCell::OASISCell(OASISData& oasisData, unsigned reference): mOASISData(oasisData), mReference(reference) {

}

OASISCell::OASISCell(OASISData& oasisData, std::string& name): mOASISData(oasisData), mName(name) {

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
    case 18:
    {
        CellElement* placement = new Placement(type);
        placement->parse(fileStream, layerSet);
        mCellElements.push_back(placement);
        break;
    }
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

const BoundingBox& OASISCell::calculateBoundingBox() {
    for (CellElement* element : mCellElements) {
        cout << "Cell Element:" << element->elementName() << endl;
        Rectangle* rectangle = dynamic_cast<Rectangle*>(element);
        if (rectangle != nullptr) {
            mBoundingBox.lx = min(mBoundingBox.lx, rectangle->getLX());
            mBoundingBox.ly = min(mBoundingBox.ly, rectangle->getLY());
            mBoundingBox.mx = max(mBoundingBox.mx, rectangle->getMX());
            mBoundingBox.my = max(mBoundingBox.my, rectangle->getMY());
            continue;
        }
        Trapezoid* trapezoid = dynamic_cast<Trapezoid*>(element);
        if (trapezoid != nullptr) {
            mBoundingBox.lx = min(mBoundingBox.lx, trapezoid->getLX());
            mBoundingBox.ly = min(mBoundingBox.ly, trapezoid->getLY());
            mBoundingBox.mx = max(mBoundingBox.mx, trapezoid->getMX());
            mBoundingBox.my = max(mBoundingBox.my, trapezoid->getMY());
            continue;
        }
        CTrapezoid* cTrapezoid = dynamic_cast<CTrapezoid*>(element);
        if (cTrapezoid != nullptr) {
            continue;
        }
        Placement* placement = dynamic_cast<Placement*>(element);
        if (placement != nullptr) {
            BoundingBox bBox = placement->calculateBoundingBox(mOASISData);
            if (bBox.lx != INT_MAX) {
                mBoundingBox.lx = min(mBoundingBox.lx, bBox.lx);
                mBoundingBox.ly = min(mBoundingBox.ly, bBox.ly);
                mBoundingBox.mx = max(mBoundingBox.mx, bBox.mx);
                mBoundingBox.my = max(mBoundingBox.my, bBox.my);
            }
            continue;
        }
    }
    cout << mName << "(" << mBoundingBox.lx << "," << mBoundingBox.ly << ") - (" << mBoundingBox.mx << "," << mBoundingBox.my << ")" << endl;
    return mBoundingBox;
}

OASISCell::~OASISCell() {
    for (CellElement* element : mCellElements) {
        delete element;
    }
    mCellElements.clear();
}

}