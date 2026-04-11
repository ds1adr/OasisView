#ifndef OASISVIEW_H
#define OASISVIEW_H

#include <QWidget>

class OASISView: public QWidget
{
public:
    OASISView();
    ~OASISView();

    void paintEvent(QPaintEvent* event) override;
};

#endif // OASISVIEW_H
