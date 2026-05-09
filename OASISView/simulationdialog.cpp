#include "simulationdialog.h"

#include <QDoubleValidator>

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
}