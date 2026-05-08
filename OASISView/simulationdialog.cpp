#include "simulationdialog.h"

#include <QDoubleValidator>

SimulationDialog::SimulationDialog(QWidget *parent)
    : QDialog{parent}
{
    mMainLayout = new QVBoxLayout(this);
    mLowLeftLayout = new QHBoxLayout();
    mMainLayout->addLayout(mLowLeftLayout);

    mLowLeftLabel = new QLabel();
    mLowLeftLabel->setText("Lower Left Point:");
    mLowLeftLayout->addWidget(mLowLeftLabel);

    mLowLeftX = new QLineEdit();
    mLowLeftX->setValidator(new QDoubleValidator());
    mLowLeftLayout->addWidget(mLowLeftX);

    mLowLeftY = new QLineEdit();
    mLowLeftY->setValidator(new QDoubleValidator());
    mLowLeftLayout->addWidget(mLowLeftY);

    mUpperRightLayout = new QHBoxLayout();
    mMainLayout->addLayout(mUpperRightLayout);

    mUpperRightLabel = new QLabel();
    mUpperRightLabel->setText("Upper Right Point:");
    mUpperRightLayout->addWidget(mUpperRightLabel);

    mUpperRightX = new QLineEdit();
    mUpperRightX->setValidator(new QDoubleValidator());
    mUpperRightLayout->addWidget(mUpperRightX);

    mUpperRightY = new QLineEdit();
    mUpperRightY->setValidator(new QDoubleValidator());
    mUpperRightLayout->addWidget(mUpperRightY);

}

SimulationDialog::~SimulationDialog() {

}