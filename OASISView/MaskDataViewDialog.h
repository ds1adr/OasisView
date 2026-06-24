#ifndef MASKDATAVIEWDIALOG_H
#define MASKDATAVIEWDIALOG_H

#include <QDialog>

#include "Simulator/Simulator.h"

class MaskDataViewDialog : public QDialog
{
    Q_OBJECT

private:
    SimulationConfig config;
    std::vector<double> mask;
public:
    explicit MaskDataViewDialog(QWidget *parent = nullptr);

    void updateMask(SimulationConfig& config, std::vector<double>& mask);

    void paintEvent(QPaintEvent *event) override;

signals:

};

#endif // MASKDATAVIEWDIALOG_H
