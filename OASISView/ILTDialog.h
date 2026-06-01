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
private slots:
    void runButtonClicked(bool);
    void cancelButtonClicked(bool);
signals:
    void cancelSelected();
    void ILTSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma, float innerSigma);
};

#endif // ILTDIALOG_H
