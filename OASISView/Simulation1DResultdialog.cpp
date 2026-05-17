#include "Simulation1DResultdialog.h"

Simulation1DResultDialog::Simulation1DResultDialog(QWidget *parent)
    : QWidget{parent}
{
    mMainLayout = new QVBoxLayout(this);
    mChartView = new QChartView();
    mMainLayout->addWidget(mChartView);
}
