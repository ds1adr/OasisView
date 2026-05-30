#ifndef CUSIMULATOR_H
#define CUSIMULATOR_H

#include <cufft.h>
#include <cuda_runtime.h>

#include "Simulator.h"

void cu_simulate_2d_abbe(const SimulationConfig& c, double* mask, std::vector<double>& total_intensity);

#endif // CUSIMULATOR_H
