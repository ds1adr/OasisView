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

    mFlipGridLabel = new QLabel();
    mFlipGridLabel->setText("Flip Grid (nm):");

    mFlipGrid = new QLineEdit();
    mFlipGrid->setValidator(intValidator);

    mExposureLayout->addWidget(mFlipGridLabel, 5, 0);
    mExposureLayout->addWidget(mFlipGrid, 5, 1);

    mMaxCountLabel = new QLabel();
    mMaxCountLabel->setText("Max ILT Count:");

    mMaxCount = new QLineEdit();
    mMaxCount->setValidator(intValidator);

    mExposureLayout->addWidget(mMaxCountLabel, 6, 0);
    mExposureLayout->addWidget(mMaxCount, 6,1);

    delete intValidator;
}

ILTDialog::~ILTDialog() {
    delete mHLine;

    delete mFlipGridLabel;
    delete mFlipGrid;

    delete mMaxCountLabel;
    delete mMaxCount;
}

void ILTDialog::runButtonClicked(bool) {
    std::cout << "ILT RunButton" << std::endl;
}

void ILTDialog::cancelButtonClicked(bool) {
    std::cout << "ILT Cancel Button" << std::endl;
}
