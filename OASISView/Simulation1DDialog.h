#ifndef SIMULATION1DDIALOG_H
#define SIMULATION1DDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class Simulation1DDialog : public QDialog
{
    Q_OBJECT
public:
    explicit Simulation1DDialog(QWidget *parent = nullptr);
    ~Simulation1DDialog();

private:
    QVBoxLayout* mMainLayout;

    QHBoxLayout* mDesignLayout;
    QLabel* mPitchLabel;
    QLineEdit* mPitch;
    QLabel* mSpaceLabel;
    QLineEdit* mSpace;

    QHBoxLayout* mWindowLayout;
    QLabel* mSimWindowLabel;
    QLineEdit* mSimWindow;

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
    void runButtonClicked(bool);
    void cancelButtonClicked(bool);
signals:
    void cancelSelected();
    void simulation1DSelected(int pitch, int space, int simulationWindow, float waveLength, float na, float sigma);
};

#endif // SIMULATION1DDIALOG_H
