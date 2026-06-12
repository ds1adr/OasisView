#ifndef CUSIMULATOR_H
#define CUSIMULATOR_H

#ifdef _CUDA_

#include <cufft.h>
#include <cuda_runtime.h>

#include "Simulator.h"

void cu_simulate_2d_abbe(const SimulationConfig& c, std::vector<double>& mask, std::vector<double>& total_intensity);

#endif

#endif // CUSIMULATOR_H
