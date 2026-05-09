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
    QLabel* mLowLeftXUnitLabel;
    QLineEdit* mLowLeftY;
    QLabel* mLowLeftYUnitLabel;
    QLabel* mUpperRightLabel;
    QLineEdit* mUpperRightX;
    QLabel* mUpperRightXUnitLabel;
    QLineEdit* mUpperRightY;
    QLabel* mUpperRightYUnitLabel;

    QFrame* mLine;

signals:
};

#endif // SIMULATIONDIALOG_H
