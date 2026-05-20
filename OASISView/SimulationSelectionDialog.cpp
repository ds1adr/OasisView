#include "SimulationSelectionDialog.h"

#include <QPushButton>

SimulationSelectionDialog::SimulationSelectionDialog(QWidget *parent)
    : QDialog{parent}
{
    mMainLayout = new QVBoxLayout(this);

    mLabel = new QLabel();
    mLabel->setText("Select Simulation Dimension");
    mMainLayout->addWidget(mLabel);

    mButtonsLayout = new QHBoxLayout();
    mMainLayout->addLayout(mButtonsLayout);

    m1DButton = new QPushButton();
    m1DButton->setText("1D");
    mButtonsLayout->addWidget(m1DButton);

    m2DButton = new QPushButton();
    m2DButton->setText("2D: Viewer");
    mButtonsLayout->addWidget(m2DButton);

    mCancelButton = new QPushButton();
    mCancelButton->setText("Cancel");
    mButtonsLayout->addWidget(mCancelButton);
}
