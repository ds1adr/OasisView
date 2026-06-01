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

    QGridLayout* mExposureLayout;

    QLabel* mWaveLengthLabel;
    QLineEdit* mWaveLength;
    QLabel* mNALabel;
    QLineEdit* mNA;
    QLabel* mSigmaLabel;
    QLineEdit* mSigma;
    QLabel* mInnerSigmaLabel;
    QLineEdit* mInnerSigma;

    QHBoxLayout* mButtonsLayout;
    QPushButton* mCancelButton;
    QPushButton* mRunButton;

private slots:
    virtual void runButtonClicked(bool);
    virtual void cancelButtonClicked(bool);
signals:
    void cancelSelected();
    void simulationSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma, float innerSigma);
};

#endif // SIMULATIONDIALOG_H
