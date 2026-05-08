#ifndef SIMULATIONDIALOG_H
#define SIMULATIONDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class SimulationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SimulationDialog(QWidget *parent = nullptr);
    ~SimulationDialog();

private:
    QVBoxLayout* mMainLayout;
    QHBoxLayout* mLowLeftLayout;
    QHBoxLayout* mUpperRightLayout;

    QLabel* mLowLeftLabel;
    QLineEdit* mLowLeftX;
    QLineEdit* mLowLeftY;
    QLabel* mUpperRightLabel;
    QLineEdit* mUpperRightX;
    QLineEdit* mUpperRightY;

signals:
};

#endif // SIMULATIONDIALOG_H
