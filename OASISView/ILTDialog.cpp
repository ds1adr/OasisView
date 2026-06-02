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
}

ILTDialog::~ILTDialog() {

}

void ILTDialog::runButtonClicked(bool) {
    std::cout << "ILT RunButton" << std::endl;
}

void ILTDialog::cancelButtonClicked(bool) {
    std::cout << "ILT Cancel Button" << std::endl;
}
