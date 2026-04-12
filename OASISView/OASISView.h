#ifndef OASISVIEW_H
#define OASISVIEW_H

#include "OASISParser/OASISCells.h"

#include <QWidget>

class OASISView: public QWidget
{
public:
    OASISView();
    ~OASISView();

    void paintEvent(QPaintEvent* event) override;
    void updateCell(OASISParser::OASISCell* cell);
};

#endif // OASISVIEW_H
