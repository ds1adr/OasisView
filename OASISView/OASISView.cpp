#include "OASISView.h"

#include <QPainter>

using namespace OASISParser;

OASISView::OASISView()
{

}

OASISView::~OASISView() {

}

void OASISView::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);

    QRect rect(1, 1, 2, 2);

    painter.end();
}

void OASISView::updateCell(OASISCell* cell) {
    QSize viewSize = this->size();
    BoundingBox bBox = cell->getBoundingBox();
    float ratio = std::min(width() / (bBox.maxX - bBox.minX), height() / (bBox.maxY - bBox.minY));
    const std::vector<CellElement*> elements = cell->getCellElements();

    for (CellElement* element: elements) {

        Rectangle* rectangle = dynamic_cast<Rectangle*>(element);
        if (rectangle != nullptr) {
            // mBoundingBox.lx = min(mBoundingBox.lx, rectangle->getLX());
            // mBoundingBox.ly = min(mBoundingBox.ly, rectangle->getLY());
            // mBoundingBox.mx = max(mBoundingBox.mx, rectangle->getMX());
            // mBoundingBox.my = max(mBoundingBox.my, rectangle->getMY());
            continue;
        }
        Trapezoid* trapezoid = dynamic_cast<Trapezoid*>(element);
        if (trapezoid != nullptr) {
            // mBoundingBox.lx = min(mBoundingBox.lx, trapezoid->getLX());
            // mBoundingBox.ly = min(mBoundingBox.ly, trapezoid->getLY());
            // mBoundingBox.mx = max(mBoundingBox.mx, trapezoid->getMX());
            // mBoundingBox.my = max(mBoundingBox.my, trapezoid->getMY());
            continue;
        }
        CTrapezoid* cTrapezoid = dynamic_cast<CTrapezoid*>(element);
        if (cTrapezoid != nullptr) {
            continue;
        }
        Placement* placement = dynamic_cast<Placement*>(element);
        if (placement != nullptr) {
            // BoundingBox bBox = placement->calculateBoundingBox(mOASISData);
            // if (bBox.lx != INT_MAX) {
            //     mBoundingBox.lx = min(mBoundingBox.lx, bBox.lx);
            //     mBoundingBox.ly = min(mBoundingBox.ly, bBox.ly);
            //     mBoundingBox.mx = max(mBoundingBox.mx, bBox.mx);
            //     mBoundingBox.my = max(mBoundingBox.my, bBox.my);
            // }
            continue;
        }
    }
}