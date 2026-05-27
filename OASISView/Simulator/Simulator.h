#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <fftw3.h>
#include <vector>

enum class PupilType {
    conventional,
    angular,
    quardrupole,
    dipole_x,
    dipole_y
};

struct SimulationConfig {
    PupilType pupil = PupilType::conventional;
    double wavelength = 193.0;
    double NA = 1.35;
    double sigma = 0.5; // Partial coherence
    double innerSigma = 0;
    // int N = 512;        // Simulation grid size (NxN)
    int Nx = 512;
    int Ny = 512;
    double dx = 1.0, dy = 1.0;
};

struct SimulationConfig1D {
    double wavelength = 193.0;
    double NA = 1.35;
    double sigma = 0.5; // Partial coherence
    int N = 512;        // Simulation grid size (NxN)
    double dx = 0.1;
    double simulationWidth = 200;

    int pitch = 200; // Unit: nm
    int spaceWidth = 100;  // Unit: nm
};

void simulate_2d_abbe(const SimulationConfig& c, double* mask, std::vector<double>& total_intensity);
void simulate_1d(const SimulationConfig1D& c, std::vector<double>& mask, std::vector<double>& oSpectrum, std::vector<double>& total_intensity);
void make1DData(fftw_complex* mask, const SimulationConfig1D* config);

#endif // SIMULATOR_H
