#ifndef SIMULATION1DRESULTDIALOG_H
#define SIMULATION1DRESULTDIALOG_H

#include <QDialog>
#include <QChart>
#include <QChartView>
#include <QVBoxLayout>

#include "Simulator/Simulator.h"

class Simulation1DResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit Simulation1DResultDialog(QWidget *parent = nullptr);

    void setValues(const SimulationConfig1D& config, const std::vector<double>values);
private:
    QVBoxLayout* mMainLayout;
    QChartView* mChartView;
    QChart* mChart;

signals:
};

#endif // SIMULATION1DRESULTDIALOG_H
