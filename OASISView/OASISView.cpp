#include "OASISParser/OASISElement.h"
#include "OASISView.h"
#include "OASISParser/OASISData.h"

#include <iostream>
#include <variant>
#include <QPainter>

using namespace OASISParser;
using namespace std;

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
    mRatio = std::min((float)width() / (float)(mBoundBox.maxX - mBoundBox.minX), (float)height() / (float)(mBoundBox.maxY - mBoundBox.minY));

    const std::vector<CellElement*> elements = mCell->getCellElements();

    for (CellElement* element: elements) {

        Rectangle* rectangle = dynamic_cast<Rectangle*>(element);
        if (rectangle != nullptr) {
            drawRectangle(painter, rectangle);
            continue;
        }
        Trapezoid* trapezoid = dynamic_cast<Trapezoid*>(element);
        if (trapezoid != nullptr) {
            drawTrapezoid(painter, trapezoid);
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
            std::cout << "Sub Cell bbox:" << subCellBBox.minX << "," << subCellBBox.minY << "," << subCellBBox.maxX << "," << subCellBBox.maxY << std::endl;
            int drawingWidth = subCell->getBoundingWidth() * mRatio;
            int drawingHeight = subCell->getBoundingHeight() * mRatio;

            std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> repetition = placement->getRepetition();
            if (holds_alternative<Repetition>(repetition)) {
                Repetition r = get<Repetition>(repetition);

                int placeX = placement->getX();
                int placeY = placement->getY() + subCell->getBoundingHeight(); // QRect origin is top left

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
        Polygon* polygon = dynamic_cast<Polygon*>(element);
        if (polygon != nullptr) {
            drawPolygon(painter, polygon);
        }
    }
}

void OASISView::drawRectangle(QPainter& painter, OASISParser::Rectangle* rectangle) {
    int initX = rectangle->getMinX();
    int initY = rectangle->getMaxY();  // QRect type origin is upper left
    int w = (int)(rectangle->getWidth() * mRatio);
    int h = (int)(rectangle->getHeight() * mRatio);

    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> repetition = rectangle->getRepetition();
    if (holds_alternative<Repetition>(repetition)) {
        Repetition r = get<Repetition>(repetition);
        if (r.nx == 0 && r.ny == 0) {
            QPoint p = calculatePoint(initX, initY);
            QRect r = QRect(p.x(), p.y(), w, h);
            painter.drawRect(r);
        } else {
            for (int i = 0; i < r.nx; i++) {
                for (int j = 0; j < r.ny; j++) {
                    QPoint p = calculatePoint(initX + r.dx * i, initY + r.dy * j);
                    QRect r = QRect(p.x(), p.y(), w, h);
                    painter.drawRect(r);
                }
            }
        }
    } else {
        QPoint p = calculatePoint(initX, initY);
        QRect r = QRect(p.x(), p.y(), w, h);
        painter.drawRect(r);
    }
    // TODO: Another type of Repetition
}

void OASISView::drawTrapezoid(QPainter& painter, OASISParser::Trapezoid* trapezoid) {
    QPolygon polygon;
    // TODO: Repetition
    const std::vector<KPoint> points = trapezoid->getInitialPoints();
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> repetition = trapezoid->getRepetition();

    // TODO: Another type of Repetition
    if (holds_alternative<Repetition>(repetition)) {
        Repetition r = get<Repetition>(repetition);
        if (r.nx == 0 && r.ny == 0) {
            for (KPoint p : points) {
                QPoint qP = calculatePoint(p.x, p.y);
                polygon << qP;
            }
            painter.drawPolygon(polygon);
        } else {
            for (int i = 0; i < r.nx; i++) {
                for (int j = 0; j < r.ny; j++) {
                    for (KPoint p : points) {
                        QPoint qP = calculatePoint(p.x + r.dx * i, p.y + r.dy * j);
                        polygon << qP;
                    }
                    painter.drawPolygon(polygon);
                }
            }
        }
    } else {
        for (KPoint p : points) {
            QPoint qP = calculatePoint(p.x, p.y);
            polygon << qP;
        }
        painter.drawPolygon(polygon);
    }
}

void OASISView::drawCTrapezoid(QPainter& painter, CTrapezoid* cTrapezoid) {
    QPolygon polygon;
    const std::vector<KPoint> points = cTrapezoid->getInitialPoints();
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> repetition = cTrapezoid->getRepetition();

    // TODO: Repetition
    if (holds_alternative<Repetition>(repetition)) {
        Repetition r = get<Repetition>(repetition);
        if (r.nx == 0 && r.ny == 0) {
            for (KPoint p : points) {
                QPoint qP = calculatePoint(p.x, p.y);
                polygon << qP;
            }
            painter.drawPolygon(polygon);
        } else {
            for (int i = 0; i < r.nx; i++) {
                for (int j = 0; j < r.ny; j++) {
                    for (KPoint p : points) {
                        QPoint qP = calculatePoint(p.x + r.dx * i, p.y + r.dy * j);
                        polygon << qP;
                    }
                    painter.drawPolygon(polygon);
                }
            }
        }
    } else {
        for (KPoint p : points) {
            QPoint qP = calculatePoint(p.x, p.y);
            polygon << qP;
        }
        painter.drawPolygon(polygon);
    }
}

void OASISView::drawPolygon(QPainter& painter, OASISParser::Polygon* _polygon) {
    QPolygon polygon;
    const vector<KPoint> points = _polygon->getInitialPoints();
    std::variant<Repetition, NSpaceRepetition, DiagonalRepetition, NDisplacementRepetition> repetition = _polygon->getRepetition();

    if (holds_alternative<Repetition>(repetition)) {
        Repetition r = get<Repetition>(repetition);
        if (r.nx == 0 && r.ny == 0) {
            for (KPoint p : points) {
                QPoint qP = calculatePoint(p.x, p.y);
                polygon << qP;
            }
            painter.drawPolygon(polygon);
        } else {
            for (int i = 0; i < r.nx; i++) {
                for (int j = 0; j < r.ny; j++) {
                    for (KPoint p : points) {
                        QPoint qP = calculatePoint(p.x + r.dx * i, p.y + r.dy * j);
                        polygon << qP;
                    }
                    painter.drawPolygon(polygon);
                }
            }
        }
    } else {
        for (KPoint p : points) {
            QPoint qP = calculatePoint(p.x, p.y);
            polygon << qP;
        }
        painter.drawPolygon(polygon);
    }
}

QPoint OASISView::calculatePoint(int x, int y) {
    float tX = (float)(x - mBoundBox.minX) * mRatio;
    float tY = (float)(mBoundBox.maxY - y) * mRatio;
    return QPoint((int)tX, (int) tY);
}
