#include "simulationdialog.h"

#include <QDoubleValidator>
#include <QPushButton>

SimulationDialog::SimulationDialog(QWidget *parent)
    : QDialog{parent}
{
    mMainLayout = new QVBoxLayout(this);
    mLowLeftLayout = new QHBoxLayout();
    mMainLayout->addLayout(mLowLeftLayout);

    mLowLeftLabel = new QLabel();
    mLowLeftLabel->setText("Lower Left Point, X:");
    mLowLeftLayout->addWidget(mLowLeftLabel);

    mLowLeftX = new QLineEdit();
    mLowLeftX->setValidator(new QDoubleValidator());
    mLowLeftLayout->addWidget(mLowLeftX);

    mLowLeftXUnitLabel = new QLabel();
    mLowLeftXUnitLabel->setText("nm, Y:");
    mLowLeftLayout->addWidget(mLowLeftXUnitLabel);

    mLowLeftY = new QLineEdit();
    mLowLeftY->setValidator(new QDoubleValidator());
    mLowLeftLayout->addWidget(mLowLeftY);

    mLowLeftYUnitLabel = new QLabel();
    mLowLeftYUnitLabel->setText("nm");
    mLowLeftLayout->addWidget(mLowLeftYUnitLabel);

    mUpperRightLayout = new QHBoxLayout();
    mMainLayout->addLayout(mUpperRightLayout);

    mUpperRightLabel = new QLabel();
    mUpperRightLabel->setText("Upper Right Point X:");
    mUpperRightLayout->addWidget(mUpperRightLabel);

    mUpperRightX = new QLineEdit();
    mUpperRightX->setValidator(new QDoubleValidator());
    mUpperRightLayout->addWidget(mUpperRightX);

    mUpperRightXUnitLabel = new QLabel();
    mUpperRightXUnitLabel->setText("nm, Y:");
    mUpperRightLayout->addWidget(mUpperRightXUnitLabel);

    mUpperRightY = new QLineEdit();
    mUpperRightY->setValidator(new QDoubleValidator());
    mUpperRightLayout->addWidget(mUpperRightY);

    mUpperRightYUnitLabel = new QLabel();
    mUpperRightYUnitLabel->setText("nm");
    mUpperRightLayout->addWidget(mUpperRightYUnitLabel);

    mLine = new QFrame();
    mLine->setFrameShape(QFrame::HLine);
    mLine->setFrameShadow(QFrame::Sunken);
    mMainLayout->addWidget(mLine);

    mExposureLayout = new QGridLayout();

    mWaveLengthLabel = new QLabel();
    mWaveLengthLabel->setText("Wave Length (nm):");

    mWaveLength = new QLineEdit();
    mWaveLength->setText("193");

    mExposureLayout->addWidget(mWaveLengthLabel, 0, 0);
    mExposureLayout->addWidget(mWaveLength, 0, 1);

    mNALabel = new QLabel();
    mNALabel->setText("NA:");

    mNA = new QLineEdit();
    mNA->setText("1.4");

    mExposureLayout->addWidget(mNALabel, 1, 0);
    mExposureLayout->addWidget(mNA, 1, 1);

    mSigmaLabel = new QLabel();
    mSigmaLabel->setText("Sigma");

    mSigma = new QLineEdit();
    mSigma->setText("0.5");

    mExposureLayout->addWidget(mSigmaLabel, 2, 0);
    mExposureLayout->addWidget(mSigma, 2, 1);

    mMainLayout->addLayout(mExposureLayout);
    // QLabel* mInnerSigmaLabel;
    // QLineEdit* mInnerSigma;

    mButtonsLayout = new QHBoxLayout();

    mCancelButton = new QPushButton("Cancel");

    mRunButton = new QPushButton("Run");
    mButtonsLayout->addWidget(mCancelButton);
    mButtonsLayout->addWidget(mRunButton);

    mMainLayout->addLayout(mButtonsLayout);
}

SimulationDialog::~SimulationDialog() {
    delete mLine;
    delete mUpperRightYUnitLabel;
    delete mUpperRightY;
    delete mUpperRightXUnitLabel;
    delete mUpperRightX;
    delete mUpperRightLayout;
    delete mLowLeftYUnitLabel;
    delete mLowLeftY;
    delete mLowLeftXUnitLabel;
    delete mLowLeftX;
    delete mLowLeftLabel;

    delete mWaveLengthLabel;
    delete mWaveLength;
    delete mNALabel;
    delete mNA;
    delete mSigmaLabel;
    delete mSigma;
    delete mInnerSigmaLabel;
    delete mInnerSigma;
    delete mExposureLayout;

    delete mMainLayout;
}