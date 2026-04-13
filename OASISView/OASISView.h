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

    QPoint calculatePoint(int x, int y);  // From Design to UI
    void drawCell(QPainter& painter);
    void drawCTrapezoid(QPainter& painter, OASISParser::CTrapezoid* ctrapezoid);
public:
    OASISView();
    ~OASISView();

    void paintEvent(QPaintEvent* event) override;
    void updateCell(OASISParser::OASISData* oasisData, OASISParser::OASISCell* cell); 
};

#endif // OASISVIEW_H
