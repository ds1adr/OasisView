#include "ILTThread.h"
#include "Simulator/cuSimulator.h"

#include <random>

using namespace std;

ILTThread::ILTThread(SimulationConfig& config, vector<double>& mask, float threshould, int flipGrid, int maxCount, QObject *parent)
    : QThread{parent}, config{config}, mask{mask}, threshould{threshould}, maxCount{maxCount}
{
}

void ILTThread::makeTargetIntensityFromMask(std::vector<double>& target, std::vector<double>& mask) {
    int size = config.Nx * config.Ny;

    for (int i = 0; i < size; i++) {
        if (mask[i] > 0) {
            target[i] = threshould * 2.0;
        } else {
            target[i] = 0.0;
        }
    }
}

void ILTThread::run() {
    // make mask data from QOASISData (1D array with 2D size)
    int size = config.Nx * config.Ny;
    vector<double> targetIntensity(size, 0);

    // Target Intensity
    makeTargetIntensityFromMask(targetIntensity, mask);

    // results
    vector<double> intensity(size, 0);

    // first run for reference
#ifdef _CUDA_
    cu_simulate_2d_abbe(config, mask, intensity);
#else
    simulate_2d_abbe(config, mask, intensity);
#endif
    // calculate initial cost function
    double minCost = getCost(config, targetIntensity, intensity);
    int smallDropCount = 0;

    std::vector<double> flipedMask(size, 0);
    for (int i = 0; i < size; i++) {
        flipedMask[i] = mask[i];
    }
    int count = 0;
    do {
        // flip mask randomly
        auto location = flipMask(flipedMask);

        // simulate_2d
#ifdef _CUDA_
        cu_simulate_2d_abbe(config, flipedMask, intensity);
#else
        simulate_2d_abbe(config, flipedMask, intensity);
#endif

        // calculate cost function
        double cost = getCost(config, targetIntensity, intensity);

        // if cost function is larger than before, roll back
        if (cost < minCost) {
            double percent = (minCost - cost)/minCost;
            // Need to proper value / method to catch saturation point
//            if (percent < 0.03) {
//                smallDropCount++;
//                if (smallDropCount == 10) {
//                    break;
//                }
//            } else {
//                smallDropCount = 0;
//            }
            minCost = cost;
        } else {
            rollbackMask(config, flipGrid, flipedMask, location);
        }
        count++;
    } while(count < maxCount);

// Emit
//    writeVectorArray(config, flipedMask);

    // Display Dialog or Widget
    // Emit
//    writeVectorArray(config, intensity);
}

std::tuple<int, int> ILTThread::flipMask(std::vector<double>& mask) {
    int countX = (int)(config.Nx * config.dx)/flipGrid;
    int countY = (int)(config.Ny * config.dy)/flipGrid;
    int gridCountX = flipGrid / config.dx;
    int gridCountY = flipGrid / config.dy;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribX(0, countX);
    std::uniform_int_distribution<int> distribY(0, countY);

    int randX = distribX(gen);
    int randY = distribY(gen);

    int x = randX * gridCountX;
    int y = randY * gridCountY;

    for (int jy = y; jy < y + gridCountY; jy++) {
        for (int ix = x; ix < x + gridCountX; ix++) {
            mask[jy * config.Nx + ix] = (mask[jy * config.Nx + ix]) > 0 ? 0.0 : 1.0 / (config.Nx * config.Ny);
        }
    }

    return std::make_tuple(x, y);
}

void ILTThread::rollbackMask(SimulationConfig& c, int flipGrid, std::vector<double>& mask, std::tuple<int, int>& locTuple) {
    int gridCountX = flipGrid / c.dx;
    int gridCountY = flipGrid / c.dy;

    int x = get<0>(locTuple);
    int y = get<1>(locTuple);

    for (int jy = y; jy < y + gridCountY; jy++) {
        for (int ix = x; ix < x + gridCountX; ix++) {
            mask[jy * c.Nx + ix] = (mask[jy * c.Nx + ix]) > 0 ? 0.0 : 1.0 / (c.Nx * c.Ny);
        }
    }
}

