#include "Simulation1DDialog.h"

#include <QIntValidator>
#include <QPushButton>

Simulation1DDialog::Simulation1DDialog(QWidget *parent)
    : QDialog{parent}
{
    QIntValidator *intValidator = new QIntValidator();
    QDoubleValidator* doubleValidator = new QDoubleValidator();

    mMainLayout = new QVBoxLayout(this);

    mDesignLayout = new QHBoxLayout();
    mPitchLabel = new QLabel();
    mPitchLabel->setText("L/S Pitch (nm):");
    mDesignLayout->addWidget(mPitchLabel);

    mPitch = new QLineEdit();
    mDesignLayout->addWidget(mPitch);

    mSpaceLabel = new QLabel();
    mSpaceLabel->setText(" Space (nm):");
    mDesignLayout->addWidget(mSpaceLabel);

    mSpace = new QLineEdit();
    mDesignLayout->addWidget(mSpace);

    mMainLayout->addLayout(mDesignLayout);

    mWindowLayout = new QHBoxLayout();
    mSimWindowLabel = new QLabel();
    mSimWindowLabel->setText("Simulation Window (nm):");
    mSimWindow = new QLineEdit();
    mSimWindow->setValidator(intValidator);
    mWindowLayout->addWidget(mSimWindowLabel);
    mWindowLayout->addWidget(mSimWindow);
    mMainLayout->addLayout(mWindowLayout);

    mExposureLayout = new QGridLayout();

    mWaveLengthLabel = new QLabel();
    mWaveLengthLabel->setText("Wave Length (nm):");

    mWaveLength = new QLineEdit();
    mWaveLength->setText("193");
    mWaveLength->setValidator(doubleValidator);

    mExposureLayout->addWidget(mWaveLengthLabel, 0, 0);
    mExposureLayout->addWidget(mWaveLength, 0, 1);

    mNALabel = new QLabel();
    mNALabel->setText("NA:");
    mNALabel->setVisible(doubleValidator);

    mNA = new QLineEdit();
    mNA->setText("1.4");

    mExposureLayout->addWidget(mNALabel, 1, 0);
    mExposureLayout->addWidget(mNA, 1, 1);

    mSigmaLabel = new QLabel();
    mSigmaLabel->setText("Sigma");

    mSigma = new QLineEdit();
    mSigma->setText("0.5");
    mSigma->setValidator(doubleValidator);

    mExposureLayout->addWidget(mSigmaLabel, 2, 0);
    mExposureLayout->addWidget(mSigma, 2, 1);

    mMainLayout->addLayout(mExposureLayout);
    // QLabel* mInnerSigmaLabel;
    // QLineEdit* mInnerSigma;

    mButtonsLayout = new QHBoxLayout();

    mCancelButton = new QPushButton("Cancel");
    connect(mCancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButtonClicked(bool)));

    mRunButton = new QPushButton("Run");
    connect(mRunButton, SIGNAL(clicked(bool)), this, SLOT(runButtonClicked(bool)));
    mButtonsLayout->addWidget(mCancelButton);
    mButtonsLayout->addWidget(mRunButton);

    mMainLayout->addLayout(mButtonsLayout);
}

Simulation1DDialog::~Simulation1DDialog() {
    delete mRunButton;
    delete mCancelButton;
    delete mButtonsLayout;

    delete mWaveLengthLabel;
    delete mWaveLength;
    delete mNALabel;
    delete mNA;
    delete mSigmaLabel;
    delete mSigma;
    delete mExposureLayout;

    delete mPitchLabel;
    delete mPitch;
    delete mSpaceLabel;
    delete mSpace;

    delete mMainLayout;
}

void Simulation1DDialog::cancelButtonClicked(bool p) {
    emit cancelSelected();
    this->reject();
}

void Simulation1DDialog::runButtonClicked(bool p) {
    // get values
    int pitch = mPitch->text().toInt();
    int space = mSpace->text().toInt();
    int simulationWindow = mSimWindow->text().toInt();
    float waveLength = mWaveLength->text().toFloat();
    float na = mNA->text().toFloat();
    float sigma = mSigma->text().toFloat();

    emit simulation1DSelected(pitch, space, simulationWindow, waveLength, na, sigma);
    this->accept();
}