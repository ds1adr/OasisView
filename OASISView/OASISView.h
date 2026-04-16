#ifndef OASISVIEW_H
#define OASISVIEW_H

#include "OASISParser/OASISCells.h"

#include <QWidget>

class OASISView: public QWidget
{
private:
    OASISParser::BoundingBox mBoundBox;
    OASISParser::OASISData* mOASISData = nullptr;
    OASISParser::OASISCell* mCell = nullptr;
    float mRatio = 1.0;

    QPoint calculatePoint(int x, int y);  // From Design to UI
    void drawCell(QPainter& painter);
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
