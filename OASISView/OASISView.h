#ifndef OASISVIEW_H
#define OASISVIEW_H

#include "OASISParser/OASISCells.h"

#include <QWidget>

class OASISView: public QWidget
{
    Q_OBJECT
private:
    OASISParser::BoundingBox mDrawBBox;  // Draw Area bounding box
    OASISParser::OASISData* mOASISData = nullptr;
    OASISParser::OASISCell* mCell = nullptr;
    OASISParser::KPoint<int> mCellOrigin = OASISParser::KPoint<int>(0,0);
    float mRatio = 1.0;
    int mMaxDrawDelpth = 1;

    QPoint mMousePress;

    QPoint calculatePoint(int x, int y);  // From Design to UI
    OASISParser::KPoint<int> calculateLayoutPoint(QPoint& p);  // From UI to Design
    void drawCell(QPainter& painter, OASISParser::OASISCell* cell, int currentDepth, OASISParser::KPoint<int> cellOrigin);
    void drawRectangle(QPainter& painter, OASISParser::Rectangle* rectangle, OASISParser::KPoint<int> offset);
    void drawTrapezoid(QPainter& painter, OASISParser::Trapezoid* trapezoid, OASISParser::KPoint<int> offset);
    void drawCTrapezoid(QPainter& painter, OASISParser::CTrapezoid* ctrapezoid, OASISParser::KPoint<int> offset);
    void drawPolygon(QPainter& painter, OASISParser::Polygon* polygon, OASISParser::KPoint<int> offset);
    void drawPlacement(QPainter& painter, OASISParser::Placement* placement, OASISParser::KPoint<int> offset, int currentDepth);

    // Mouse
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
signals:
    void statusChanged(QString& message);
public:
    OASISView();
    ~OASISView();

    void updateStatus(QString& message) {
        emit statusChanged(message);
    }

    void paintEvent(QPaintEvent* event) override;
    void updateCell(OASISParser::OASISData* oasisData, OASISParser::OASISCell* cell); 
};

#endif // OASISVIEW_H
