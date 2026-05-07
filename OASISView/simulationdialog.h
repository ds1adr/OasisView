#ifndef SIMULATIONDIALOG_H
#define SIMULATIONDIALOG_H

#include <QDialog>
#include <QLabel>

class SimulationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SimulationDialog(QWidget *parent = nullptr);
    ~SimulationDialog();

private:
    QLabel* mLowLeftLabel;
    QLabel* mUpperRightLabel;

signals:
};

#endif // SIMULATIONDIALOG_H
