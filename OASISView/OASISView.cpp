#include "OASISView.h"

#include <iostream>
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

    drawCell(painter);

    painter.end();
}

void OASISView::updateCell(OASISCell* cell) {
    mCell = cell;
    update();
}

void OASISView::drawCell(QPainter& painter) {
    if (mCell == nullptr) {
        return;
    }
    QSize viewSize = this->size();
    mBoundBox = mCell->getBoundingBox();
    unsigned bW = (mBoundBox.maxX - mBoundBox.minX);
    unsigned bH = mBoundBox.maxY - mBoundBox.minY;

    std::cout << "BBBox Width: " << bW << "BBox Height" << bH << "View Size" << viewSize.width() << "-" << viewSize.height() << std::endl;
    float ratio = std::min((float)width() / (float)(mBoundBox.maxX - mBoundBox.minX), (float)height() / (float)(mBoundBox.maxY - mBoundBox.minY));
    std::cout<< "Ratio: " << ratio << std::endl;

    const std::vector<CellElement*> elements = mCell->getCellElements();

    for (CellElement* element: elements) {

        Rectangle* rectangle = dynamic_cast<Rectangle*>(element);
        if (rectangle != nullptr) {
            QPoint p = calculatePoint(rectangle->getMinX(), rectangle->getMaxY());
            int w = (int)(rectangle->getWidth() * ratio);
            int h = (int)(rectangle->getHeight() * ratio);

            std::cout << "Point:" << p.x() << "," << p.y() << "Rect W:" << w << "Rect H:" << h;

            QRect r = QRect(p.x(), p.y(), w, h);
            painter.drawRect(r);
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

QPoint OASISView::calculatePoint(int x, int y) {
    float ratio = std::min((float)width() / (mBoundBox.maxX - mBoundBox.minX), (float)height() / (mBoundBox.maxY - mBoundBox.minY));
    float tX = (float)(x - mBoundBox.minX) * ratio;
    float tY = (float)(mBoundBox.maxY - y) * ratio;
    return QPoint((int)tX, (int) tY);
}
