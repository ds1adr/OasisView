#include "OASISView.h"

#include <QPainter>

OASISView::OASISView()
{

}

OASISView::~OASISView() {

}

void OASISView::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);


    painter.end();
}