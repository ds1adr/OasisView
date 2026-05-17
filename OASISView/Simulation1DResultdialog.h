#ifndef SIMULATION1DRESULTDIALOG_H
#define SIMULATION1DRESULTDIALOG_H

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QVBoxLayout>

class Simulation1DResultDialog : public QWidget
{
    Q_OBJECT
public:
    explicit Simulation1DResultDialog(QWidget *parent = nullptr);
private:
    QVBoxLayout* mMainLayout;
    QChartView* mChartView;
    QChart* mChart;

signals:
};

#endif // SIMULATION1DRESULTDIALOG_H
