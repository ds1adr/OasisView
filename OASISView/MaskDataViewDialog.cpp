#include "MaskDataViewDialog.h"

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
    QRect rect = this->geometry();

    double ratioX = rect.width() / (config.Nx * config.dx);
    double ratioY = rect.height() / (config.Ny * config.dy);
    double ratio = std::min(ratioX, ratioY);

    for (int y = 0; y < config.Ny; y++) {
        for (int x = 0; x < config.Nx; x++) {

        }
    }
}
