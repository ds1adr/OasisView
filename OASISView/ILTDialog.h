#ifndef ILTDIALOG_H
#define ILTDIALOG_H

#include "simulationdialog.h"

#include <QDialog>

class ILTDialog: public SimulationDialog
{
    Q_OBJECT
public:
    ILTDialog(QWidget *parent = nullptr);
    virtual ~ILTDialog();
};

#endif // ILTDIALOG_H
