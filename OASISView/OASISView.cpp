#include "OASISView.h"
#include "OASISParser/OASISData.h"

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

// TODO: need to improve this draw logic
void OASISView::updateCell(OASISParser::OASISData* oasisData, OASISCell* cell) {
    mOASISData = oasisData;
    mCell = cell;
    update();
}

// TODO: need to improve this draw logic
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
            QPolygon polygon;
            const std::vector<KPoint> points = trapezoid->getPoints();
            for (KPoint p : points) {
                QPoint qP = calculatePoint(p.x, p.y);
                polygon << qP;
            }
            painter.drawPolygon(polygon);
            continue;
        }
        CTrapezoid* cTrapezoid = dynamic_cast<CTrapezoid*>(element);
        if (cTrapezoid != nullptr) {
            drawCTrapezoid(painter, cTrapezoid);
            continue;
        }
        Placement* placement = dynamic_cast<Placement*>(element);
        if (placement != nullptr) {
            OASISCell* subCell;
            if (placement->getCellName().empty()) {
                subCell = mOASISData->getCell(placement->getReference());
            } else {
                subCell = mOASISData->getCell(placement->getCellName());
            }
            BoundingBox subCellBBox = subCell->getBoundingBox();
            int drawingWidth = subCell->getBoundingWidth() * ratio;
            int drawingHeight = subCell->getBoundingHeight() * ratio;

            std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> repetition = placement->getRepetition();
            if (holds_alternative<Repetition>(repetition)) {
                Repetition r = get<Repetition>(repetition);

                int placeX = placement->getX();
                int placeY = placement->getY();

                for (int i = 0; i < r.nx; i++) {
                    for (int j = 0; j < r.ny; j++) {
                        int x = placeX + subCellBBox.minX + r.dx * i;
                        int y = placeY + subCellBBox.minY + r.dy * j;
                        QPoint p = calculatePoint(x, y);
                        QRect rect = QRect(p.x(), p.y(), drawingWidth, drawingHeight);

                        painter.drawRect(rect);
                    }
                }
            }
            continue;
        }
    }
}

void OASISView::drawCTrapezoid(QPainter& painter, OASISParser::CTrapezoid* cTrapezoid) {

}

QPoint OASISView::calculatePoint(int x, int y) {
    float ratio = std::min((float)width() / (mBoundBox.maxX - mBoundBox.minX), (float)height() / (mBoundBox.maxY - mBoundBox.minY));
    float tX = (float)(x - mBoundBox.minX) * ratio;
    float tY = (float)(mBoundBox.maxY - y) * ratio;
    return QPoint((int)tX, (int) tY);
}
