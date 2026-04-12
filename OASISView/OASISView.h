#ifndef OASISVIEW_H
#define OASISVIEW_H

#include "OASISParser/OASISCells.h"

#include <QWidget>

class OASISView: public QWidget
{
private:
    OASISParser::BoundingBox mBoundBox;
    OASISParser::OASISCell* mCell = nullptr;

    QPoint calculatePoint(int x, int y);  // From Design to UI
    void drawCell(QPainter& painter);
public:
    OASISView();
    ~OASISView();

    void paintEvent(QPaintEvent* event) override;
    void updateCell(OASISParser::OASISCell* cell);
};

#endif // OASISVIEW_H
