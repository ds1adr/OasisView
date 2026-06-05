#include <iostream>
#include <QIntValidator>

#include "ILTDialog.h"

ILTDialog::ILTDialog(QWidget *parent)
{
    mHLine = new QFrame();
    mHLine->setFrameShape(QFrame::HLine);
    mHLine->setFrameShadow(QFrame::Sunken);

    mExposureLayout->addWidget(mHLine, 4, 0, 1, 2);

    QIntValidator *intValidator = new QIntValidator();
    QDoubleValidator* doubleValidator = new QDoubleValidator();

    mTargetThreshouldLabel = new QLabel();
    mTargetThreshouldLabel->setText("Threshould:");

    mTargetThreshould = new QLineEdit();
    mTargetThreshould->setValidator(doubleValidator);

    mExposureLayout->addWidget(mTargetThreshouldLabel, 5, 0);
    mExposureLayout->addWidget(mTargetThreshould, 5, 1);

    mFlipGridLabel = new QLabel();
    mFlipGridLabel->setText("Flip Grid (nm):");

    mFlipGrid = new QLineEdit();
    mFlipGrid->setText("5");
    mFlipGrid->setValidator(intValidator);

    mExposureLayout->addWidget(mFlipGridLabel, 6, 0);
    mExposureLayout->addWidget(mFlipGrid, 6, 1);

    mMaxCountLabel = new QLabel();
    mMaxCountLabel->setText("Max ILT Count:");

    mMaxCount = new QLineEdit();
    mMaxCount->setText("3000");
    mMaxCount->setValidator(intValidator);

    mExposureLayout->addWidget(mMaxCountLabel, 7, 0);
    mExposureLayout->addWidget(mMaxCount, 7,1);

    delete intValidator;
    delete doubleValidator;
}

ILTDialog::~ILTDialog() {
    delete mHLine;

    delete mTargetThreshouldLabel;
    delete mTargetThreshould;

    delete mFlipGridLabel;
    delete mFlipGrid;

    delete mMaxCountLabel;
    delete mMaxCount;
}

void ILTDialog::runButtonClicked(bool) {
    std::cout << "ILT RunButton" << std::endl;
    int llx = mLowLeftX->text().toInt();
    int lly = mLowLeftY->text().toInt();
    int urx = mUpperRightX->text().toInt();
    int ury = mUpperRightY->text().toInt();
    float waveLength = mWaveLength->text().toFloat();
    float na = mNA->text().toFloat();
    float sigma = mSigma->text().toFloat();
    float innerSigma = mInnerSigma->text().toFloat();
    float threshould = mTargetThreshould->text().toFloat();
    int flipGrid = mFlipGrid->text().toInt();
    int maxCount = mMaxCount->text().toInt();

    emit ILTSelected(llx, lly, urx, ury, waveLength, na, sigma, innerSigma, threshould, flipGrid, maxCount);
}

void ILTDialog::cancelButtonClicked(bool) {
    std::cout << "ILT Cancel Button" << std::endl;
}
