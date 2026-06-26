#include "MaskDataViewDialog.h"

#include <QPainter>

MaskDataViewDialog::MaskDataViewDialog(QWidget *parent)
    : QDialog{parent}
{

}

void MaskDataViewDialog::updateMask(SimulationConfig& config, std::vector<double>& mask) {
    this->config = config;
    this->mask = mask;
    update();
}

void MaskDataViewDialog::paintEvent(QPaintEvent *event) {
    if (mask.size() < config.Nx * config.Ny) {
        return;
    }
    QRect rect = this->geometry();

    double ratioX = rect.width() / (config.Nx * config.dx);
    double ratioY = rect.height() / (config.Ny * config.dy);
    int ratio = (int)std::min(ratioX, ratioY);

    QPainter painter;
    painter.begin(this);

    painter.setBrush(QBrush(Qt::blue, Qt::Dense1Pattern));

    for (int y = 0; y < config.Ny; y++) {
        for (int x = 0; x < config.Nx; x++) {
            double value = mask[y * config.Nx + x];
            if (value == 0) continue;

            int px = x * ratio;
            int py = y * ratio;

            QRect rect = QRect(px, py, ratio, ratio);
            painter.drawRect(rect);
        }
    }

    painter.end();
}
