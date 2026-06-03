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
protected:
    QLineEdit* mLowLeftX;
    QLineEdit* mLowLeftY;
    QLineEdit* mUpperRightX;
    QLineEdit* mUpperRightY;

    QLineEdit* mWaveLength;
    QLineEdit* mNA;
    QLineEdit* mSigma;
    QLineEdit* mInnerSigma;

private:
    QVBoxLayout* mMainLayout;
    QHBoxLayout* mLowLeftLayout;
    QHBoxLayout* mUpperRightLayout;

    QLabel* mLowLeftLabel;
    QLabel* mLowLeftXUnitLabel;

    QLabel* mLowLeftYUnitLabel;
    QLabel* mUpperRightLabel;

    QLabel* mUpperRightXUnitLabel;
    QLabel* mUpperRightYUnitLabel;

    QFrame* mLine;

    QLabel* mWaveLengthLabel;
    QLabel* mNALabel;
    QLabel* mSigmaLabel;
    QLabel* mInnerSigmaLabel;

    QHBoxLayout* mButtonsLayout;
    QPushButton* mCancelButton;
    QPushButton* mRunButton;
protected:
    QGridLayout* mExposureLayout;

private slots:
    virtual void runButtonClicked(bool);
    virtual void cancelButtonClicked(bool);
signals:
    void cancelSelected();
    void simulationSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma, float innerSigma);
};

#endif // SIMULATIONDIALOG_H
