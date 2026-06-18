#ifndef ILTTHREAD_H
#define ILTTHREAD_H

#include <QThread>

#include "Simulator/Simulator.h"

class ILTThread : public QThread
{
    Q_OBJECT
public:
    explicit ILTThread(SimulationConfig& config, std::vector<double>& mask, float threshould, int flipGrid, int maxCount, QObject *parent = nullptr);

private:
    SimulationConfig& config;
    std::vector<double>& mask;
    float threshould;
    int flipGrid;
    int maxCount;

    void makeTargetIntensityFromMask(std::vector<double>& target, std::vector<double>& mask);
    std::tuple<int, int> flipMask(std::vector<double>& mask);
    void rollbackMask(SimulationConfig& c, int flipGrid, std::vector<double>& mask, std::tuple<int, int>& locTuple);
protected:
    void run() override;

signals:
    void maskUpdate(SimulationConfig& c, std::vector<double>& values);
    void intensityUpdate(SimulationConfig& c, std::vector<double>& values);
};

#endif // ILTTHREAD_H
