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
    connect(m1DButton, SIGNAL(pressed()), this, SLOT(simulation1DButtonPressed()));
    mButtonsLayout->addWidget(m1DButton);

    m2DButton = new QPushButton();
    m2DButton->setText("2D: Viewer");
    connect(m2DButton, SIGNAL(pressed()), this, SLOT(simulation2DButtonPressed()));
    mButtonsLayout->addWidget(m2DButton);

    mCancelButton = new QPushButton();
    mCancelButton->setText("Cancel");
    connect(mCancelButton, SIGNAL(pressed()), this, SLOT(simulationCancelPressed()));
    mButtonsLayout->addWidget(mCancelButton);
}

void SimulationSelectionDialog::simulation1DButtonPressed() {
    emit simulation1DButtonClicked();
    this->accept();
}

void SimulationSelectionDialog::simulation2DButtonPressed() {
    emit simulation2DButtonClicked();
    this->accept();
}

void SimulationSelectionDialog::simulationCancelPressed() {
    emit simulationCancelClicked();
    this->reject();
}