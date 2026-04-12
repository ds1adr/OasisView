#include "OASISView.h"

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

    QRect rect(1, 1, 2, 2);

    painter.end();
}

void OASISView::updateCell(OASISCell* cell) {
    // BoundingBox bBox = cell->getBoundingBox();
}