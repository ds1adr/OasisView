#include "Simulation1DResultdialog.h"
#include <QLineSeries>

Simulation1DResultDialog::Simulation1DResultDialog(QWidget *parent)
    : QDialog{parent}
{
    mMainLayout = new QVBoxLayout(this);
    mChartView = new QChartView();
    mMainLayout->addWidget(mChartView);
}

void Simulation1DResultDialog::setValues(const SimulationConfig1D& config, const std::vector<double>& values) {
    QLineSeries* series = new QLineSeries();
    for (int x = 0; x < config.N; x++) {
        series->append(x, values[x]);
    }

    QChart * chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();

    mChartView->setChart(chart);
}
