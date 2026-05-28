#include "OASISView.h"
#include "OASISParser/OASISData.h"

#include <QMouseEvent>
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

    drawCell(painter, mCell, 0, KPoint<int>(0,0));

    painter.end();
}

void OASISView::setMaxDrawDepth(int depth) {
    mMaxDrawDelpth = depth;
    update();
}

// TODO: need to improve this draw logic
void OASISView::updateCell(OASISParser::OASISData* oasisData, OASISCell* cell) {
    mOASISData = oasisData;
    mCell = cell;
    mDrawBBox = cell->getBoundingBox();
    update();
}

void OASISView::makeMaskData(SimulationConfig& c, double* mask, OASISCell* cell) {
    makeCellData(c, mask, cell, KPoint<int>(0, 0));
}

void OASISView::makeCellData(SimulationConfig& c, double* mask, OASISParser::OASISCell* cell, OASISParser::KPoint<int> cellOrigin) {
    if (cell == nullptr) {
        return;
    }

    const std::vector<CellElement*> elements = cell->getCellElements();

    for (CellElement* element: elements) {

        Rectangle* rectangle = dynamic_cast<Rectangle*>(element);
        if (rectangle != nullptr) {
            makeRectangleData(c, mask, rectangle, cellOrigin);
            continue;
        }
        Trapezoid* trapezoid = dynamic_cast<Trapezoid*>(element);
        if (trapezoid != nullptr) {
            // drawTrapezoid(painter, trapezoid, cellOrigin);
            continue;
        }
        CTrapezoid* cTrapezoid = dynamic_cast<CTrapezoid*>(element);
        if (cTrapezoid != nullptr) {
            // drawCTrapezoid(painter, cTrapezoid, cellOrigin);
            continue;
        }
        Placement* placement = dynamic_cast<Placement*>(element);
        if (placement != nullptr) {
            // drawPlacement(painter, placement, cellOrigin, currentDepth);
            continue;
        }
        Polygon* polygon = dynamic_cast<Polygon*>(element);
        if (polygon != nullptr) {
            // drawPolygon(painter, polygon, cellOrigin);
        }
    }
}

// TODO: need to improve this draw logic
void OASISView::drawCell(QPainter& painter, OASISCell* cell, int currentDepth, KPoint<int> cellOrigin) {
    if (cell == nullptr) {
        return;
    }

    mRatio = std::min((float)width() / (float)(mDrawBBox.maxX - mDrawBBox.minX), (float)height() / (float)(mDrawBBox.maxY - mDrawBBox.minY));

    const std::vector<CellElement*> elements = cell->getCellElements();

    for (CellElement* element: elements) {

        Rectangle* rectangle = dynamic_cast<Rectangle*>(element);
        if (rectangle != nullptr) {
            drawRectangle(painter, rectangle, cellOrigin);
            continue;
        }
        Trapezoid* trapezoid = dynamic_cast<Trapezoid*>(element);
        if (trapezoid != nullptr) {
            drawTrapezoid(painter, trapezoid, cellOrigin);
            continue;
        }
        CTrapezoid* cTrapezoid = dynamic_cast<CTrapezoid*>(element);
        if (cTrapezoid != nullptr) {
            drawCTrapezoid(painter, cTrapezoid, cellOrigin);
            continue;
        }
        Placement* placement = dynamic_cast<Placement*>(element);
        if (placement != nullptr) {
            drawPlacement(painter, placement, cellOrigin, currentDepth);
            continue;
        }
        Polygon* polygon = dynamic_cast<Polygon*>(element);
        if (polygon != nullptr) {
            drawPolygon(painter, polygon, cellOrigin);
        }
    }
}

void OASISView::makeRectangleData(SimulationConfig& c, double* mask, Rectangle* r, KPoint<int> offset) {
    int mask_lx = c.originX;
    int mask_ly = c.originY;
    int mask_ux = c.originX + c.Nx * c.dx;
    int mask_uy = c.originY + c.Ny * c.dy;

    int initX = r->getMinX() + offset.x();
    int initY = r->getMinY() + offset.y();
    int w = r->getWidth();
    int h = r->getHeight();

    BaseRepetition repetition = r->getRepetition();

    for (int i = 0; i < repetition.nx(); i++) {
        for (int j = 0; j < repetition.ny(); j++) {
            KPoint<int> pos = repetition.getPosition(i, j);

            int rect_lx = initX + pos.x();
            int rect_ly = initY + pos.y();
            int rect_ux = rect_lx + w;
            int rect_uy = rect_ly + h;

            if (rect_ux < mask_lx || rect_lx > mask_ux || rect_uy < mask_ly || rect_ly > mask_uy) {
                continue;
            }

            // TODO: need to improve logic
            for (int y = 0; y < c.Ny; y++) {
                for (int x = 0; x < c.Nx; x++) {
                    double cx = mask_lx + x * c.dx + c.dx / 2.0;
                    double cy = mask_ly + y * c.dy + c.dy / 2.0;

                    if (cx > rect_lx && cx < rect_ux && cy > rect_ly && cy < rect_uy) {
                        mask[y * c.Nx + x] = 1.0 / (c.Nx * c.Ny);
                    }
                }
            }
        }
    }
}

void OASISView::drawRectangle(QPainter& painter, OASISParser::Rectangle* rectangle, KPoint<int> offset) {
    int initX = rectangle->getMinX() + offset.x();
    int initY = rectangle->getMinY() + offset.y();
    int w = rectangle->getWidth();
    int h = rectangle->getHeight();

    // TODO: Another type of Repetition
    BaseRepetition repetition = rectangle->getRepetition();

    for (int i = 0; i < repetition.nx(); i++) {
        for (int j = 0; j < repetition.ny(); j++) {
            KPoint<int> pos = repetition.getPosition(i, j);

            QPolygon polygon;
            int iX = initX + pos.x();
            int iY = initY + pos.y();
            QPoint p = calculatePoint(iX, iY);
            polygon << p;
            polygon << QPoint(calculatePoint(iX + w, iY));
            polygon << QPoint(calculatePoint(iX + w, iY + h));
            polygon << QPoint(calculatePoint(iX, iY + h));
            painter.drawPolygon(polygon);
        }
    }
}

void OASISView::drawTrapezoid(QPainter& painter, OASISParser::Trapezoid* trapezoid, KPoint<int> offset) {
    const std::vector<KPoint<int>> points = trapezoid->getInitialPoints();
    BaseRepetition repetition = trapezoid->getRepetition();

    // TODO: Another type of Repetition
    for (int i = 0; i < repetition.nx(); i++) {
        for (int j = 0; j < repetition.ny(); j++) {
            QPolygon polygon;
            KPoint<int> pos = repetition.getPosition(i, j);
            for (KPoint<int> p : points) {
                QPoint qP = calculatePoint(p.x() + offset.x() + pos.x(), p.y() + offset.y() + pos.y());
                polygon << qP;
            }
            painter.drawPolygon(polygon);
        }
    }
}

void OASISView::drawCTrapezoid(QPainter& painter, CTrapezoid* cTrapezoid, OASISParser::KPoint<int> offset) {
    const std::vector<KPoint<int>> points = cTrapezoid->getInitialPoints();
    BaseRepetition repetition = cTrapezoid->getRepetition();

    for (int i = 0; i < repetition.nx(); i++) {
        for (int j = 0; j < repetition.ny(); j++) {
            KPoint<int> pos = repetition.getPosition(i, j);

            QPolygon polygon;
            for (KPoint<int> p : points) {
                QPoint qP = calculatePoint(p.x() + offset.x() + pos.x(), p.y() + offset.y() + pos.y());
                polygon << qP;
            }
            painter.drawPolygon(polygon);
        }
    }
}

void OASISView::drawPolygon(QPainter& painter, OASISParser::Polygon* _polygon, OASISParser::KPoint<int> offset) {
    const vector<KPoint<int>> points = _polygon->getInitialPoints();
    BaseRepetition repetition = _polygon->getRepetition();

    for (int i = 0; i < repetition.nx(); i++) {
        for (int j = 0; j < repetition.ny(); j++) {
            KPoint<int> pos = repetition.getPosition(i, j);
            QPolygon polygon;
            for (KPoint<int> p : points) {
                QPoint qP = calculatePoint(p.x() + offset.x() + pos.x(), p.y() + offset.y() + pos.y());
                polygon << qP;
            }
            painter.drawPolygon(polygon);
        }
    }
}

void OASISView::makePlacementData(SimulationConfig& c, double* mask, Placement* placement, KPoint<int> offset) {
    OASISCell* subCell;
    if (placement->getCellName().empty()) {
        subCell = mOASISData->getCell(placement->getReference());
    } else {
        subCell = mOASISData->getCell(placement->getCellName());
    }
    BoundingBox subCellBBox = subCell->getBoundingBox();
    subCellBBox = placement->getRotatedBoundingBox(subCellBBox);

    BaseRepetition repetition = placement->getRepetition();

    for (int i = 0; i < repetition.nx(); i++) {
        for (int j = 0; j < repetition.ny(); j++) {
            KPoint<int> pos = repetition.getPosition(i, j);
            int x = placement->getX() + offset.x() + subCellBBox.minX + pos.x();
            int y = placement->getY() + offset.y() + subCellBBox.minY + pos.y();

            makeCellData(c, mask, subCell, KPoint<int>(x,y));
        }
    }
}

void OASISView::drawPlacement(QPainter& painter, OASISParser::Placement* placement, OASISParser::KPoint<int> offset, int currentDepth) {
    OASISCell* subCell;
    if (placement->getCellName().empty()) {
        subCell = mOASISData->getCell(placement->getReference());
    } else {
        subCell = mOASISData->getCell(placement->getCellName());
    }
    BoundingBox subCellBBox = subCell->getBoundingBox();
    subCellBBox = placement->getRotatedBoundingBox(subCellBBox);
    int drawingWidth = subCell->getBoundingWidth() * mRatio;
    int drawingHeight = subCell->getBoundingHeight() * mRatio;

    BaseRepetition repetition = placement->getRepetition();

    int placeX = placement->getX();
    int placeY = placement->getY() + subCell->getBoundingHeight(); // QRect origin is top left

    for (int i = 0; i < repetition.nx(); i++) {
        for (int j = 0; j < repetition.ny(); j++) {
            if (mMaxDrawDelpth >= currentDepth) {
                KPoint<int> pos = repetition.getPosition(i, j);
                int x = placement->getX() + offset.x() + subCellBBox.minX + pos.x();
                int y = placement->getY() + offset.y() + subCellBBox.minY + pos.y();

                drawCell(painter, subCell, currentDepth + 1, KPoint<int>(x, y));
            } else {
                KPoint<int> pos = repetition.getPosition(i, j);
                int x = placeX + subCellBBox.minX + pos.x();
                int y = placeY + subCellBBox.minY + pos.y();
                QPoint p = calculatePoint(x, y);
                QRect rect = QRect(p.x(), p.y(), drawingWidth, drawingHeight);

                painter.drawRect(rect);
            }
        }
    }
}

QPoint OASISView::calculatePoint(int x, int y) {
    float tX = (float)(x - mDrawBBox.minX) * mRatio;
    float tY = (float)(mDrawBBox.maxY - y) * mRatio;
    return QPoint((int)tX, (int) tY);
}

KPoint<int> OASISView::calculateLayoutPoint(QPoint& p) {
    float tX = (float)p.x() / mRatio + mDrawBBox.minX;
    float tY = (float)-p.y() / mRatio + mDrawBBox.maxY;
    return KPoint<int>((int)tX, (int)tY);
}

// Mouse event
void OASISView::mouseMoveEvent(QMouseEvent* event) {
    if (mDrawBBox.minX == INT_MAX) {
        return;
    }
    QPoint p = event->pos();
    KPoint<int> layoutP = calculateLayoutPoint(p);
    KPoint<int> pressP = calculateLayoutPoint(mMousePress);

    if (p.x() < 0 || p.y() < 0 || p.x() > this->size().width() || p.y() > this->size().height()) {
        return;
    }
    KPoint<int> layoutPos = calculateLayoutPoint(p);

    QString sP = QString::asprintf("(%.3f, %.3f)",(float)layoutPos.x()/mOASISData->unit(), (float)layoutPos.y()/mOASISData->unit());
    updateStatus(sP);

    if (event->buttons() & Qt::RightButton) {
        KPoint<int> delta = pressP - layoutP;
        mMousePress = p;
        mDrawBBox.minX += delta.x();
        mDrawBBox.minY += delta.y();
        mDrawBBox.maxX += delta.x();
        mDrawBBox.maxY += delta.y();
        update();
    }
}

void OASISView::mousePressEvent(QMouseEvent* event) {
    mMousePress = event->pos();
}

void OASISView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }
    QPoint p = event->pos();
    if (p.x() > mMousePress.x() && p.y() > mMousePress.y()) {  // Zoom in
        KPoint<int> p1 = calculateLayoutPoint(mMousePress);
        KPoint<int> p2 = calculateLayoutPoint(p);
        mDrawBBox.minX = p1.x();
        mDrawBBox.minY = p2.y();
        mDrawBBox.maxX = p2.x();
        mDrawBBox.maxY = p1.y();
        update();
    } else if (p.x() < mMousePress.x() && p.y() < mMousePress.y()) {  // Zoom out
        KPoint p1 = calculateLayoutPoint(mMousePress);
        KPoint p2 = calculateLayoutPoint(p);
        mDrawBBox.minX = mDrawBBox.minX - (p2.x() - mDrawBBox.minX);
        mDrawBBox.minY = mDrawBBox.minY - (p1.y() - mDrawBBox.minY);
        mDrawBBox.maxX = mDrawBBox.maxX + (mDrawBBox.maxX - p1.x());
        mDrawBBox.maxY = mDrawBBox.maxY + (mDrawBBox.maxY - p2.y());
        update();
    }
}
