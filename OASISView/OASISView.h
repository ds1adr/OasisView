#ifndef OASISVIEW_H
#define OASISVIEW_H

#include "OASISParser/OASISCells.h"

#include <QWidget>

class OASISView: public QWidget
{
private:
    OASISParser::BoundingBox mDrawBBox;  // Draw Area bounding box
    OASISParser::OASISData* mOASISData = nullptr;
    OASISParser::OASISCell* mCell = nullptr;
    OASISParser::KPoint mCellOrigin = OASISParser::KPoint(0,0);
    float mRatio = 1.0;
    int mMaxDrawDelpth = 1;

    QPoint calculatePoint(int x, int y);  // From Design to UI
    void drawCell(QPainter& painter, int currentDepth, OASISParser::KPoint cellOrigin);
    void drawRectangle(QPainter& painter, OASISParser::Rectangle* rectangle);
    void drawTrapezoid(QPainter& painter, OASISParser::Trapezoid* trapezoid);
    void drawCTrapezoid(QPainter& painter, OASISParser::CTrapezoid* ctrapezoid);
    void drawPolygon(QPainter& painter, OASISParser::Polygon* polygon);
public:
    OASISView();
    ~OASISView();

    void paintEvent(QPaintEvent* event) override;
    void updateCell(OASISParser::OASISData* oasisData, OASISParser::OASISCell* cell); 
};

#endif // OASISVIEW_H
