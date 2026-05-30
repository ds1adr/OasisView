#include <vector>
#include <complex>
#include <cstring>
#include <fftw3.h>
#include <cmath>
#include <iostream>
#include "Simulator.h"

void shift_fft_1d(fftw_complex* origin, const SimulationConfig1D& c, bool isPlus) {
    if (isPlus) {
        fftw_complex t;
        t[0] = origin[0][0];
        t[1] = origin[0][1];

        for (int i = 0; i < c.N; i++) {
            if (i == c.N -1) {
                origin[i][0] = t[0];
                origin[i][1] = t[1];
            } else {
                int j = i + 1;
                origin[i][0] = origin[j][0];
                origin[i][1] = origin[j][1];
            }
        }
    } else {
        fftw_complex t;
        t[0] = origin[c.N-1][0];
        t[1] = origin[c.N-1][1];

        for (int i = c.N-1; i >= 0; i--) {
            if (i == 0) {
                origin[i][0] = t[0];
                origin[i][1] = t[1];
            } else {
                int j = i - 1;
                origin[i][0] = origin[j][0];
                origin[i][1] = origin[j][1];
            }
        }
    }
}

void make1DData(fftw_complex* mask, const SimulationConfig1D& config) {
    double x = 0;
    for (int n = 0; n < config.N; n++) {
        int count = (x / config.pitch);
        double px = x - (count * config.pitch);
        if (px < config.spaceWidth) {
            mask[n][0] = 1.0 / config.N;
        } else {
            mask[n][0] = 0.0;
        }
        mask[n][1] = 0.0;
        x += config.dx;
    }
}

void simulate_1d(const SimulationConfig1D& c, std::vector<double>& mask, std::vector<double>& oSpectrum, std::vector<double>& total_intensity) {
    fftw_complex *mask_data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * c.N);
    fftw_complex *spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * c.N);
    fftw_complex *filtered = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * c.N);
    fftw_complex *field = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * c.N);

    // 1. Initialize Mask and compute Mask Spectrum (Forward FFT)
    // (User would fill mask_data here)
    fftw_plan p_forward = fftw_plan_dft_1d(c.N, mask_data, spectrum, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan p_backward = fftw_plan_dft_1d(c.N, filtered, field, FFTW_BACKWARD, FFTW_ESTIMATE);
    make1DData(mask_data, c);
    for (int x = 0; x < c.N; x++) {
        mask[x] = mask_data[x][0];
    }
    fftw_execute(p_forward);

    // TODO: Cut spectrum by Pupil
    const double pupilRx = (c.NA / c.wavelength) * (c.N * c.dx);
    std::cout << "Pubil Radius:" << pupilRx << std::endl;

    int sigmaCount = 0;

    // TOTO: Sigma
    for (double sx = -c.sigma; sx <= c.sigma; sx += 0.1) {
        int shiftXL = std::lround(sx * (c.NA / c.wavelength) * c.N * c.dx);
        int shiftXU = shiftXL + c.N;
        std::cout << "ShiftXL: " << shiftXL << std::endl;
        sigmaCount++;

        for (int x = 0; x < c.N; x++) {
            double dx = (double)std::min(std::abs(x - shiftXL), std::abs(x - shiftXU));
            double r = (dx * dx) / (pupilRx * pupilRx);

            if (r > 1.0) {
                filtered[x][0] = 0;
                filtered[x][1] = 0;
            } else {
                filtered[x][0] = spectrum[x][0];
                filtered[x][1] = spectrum[x][1];
            }

            oSpectrum[x] += std::sqrt(filtered[x][0] * filtered[x][0] + filtered[x][1] * filtered[x][1]);
        }
        fftw_execute(p_backward);
        for (int x = 0; x < c.N; x++) {
            total_intensity[x] += std::sqrt(field[x][0]*field[x][0] + field[x][1]*field[x][1]);
        }
    }

    for (int x = 0; x < c.N; x++) {
        total_intensity[x] /= (double)sigmaCount;
    }

    fftw_destroy_plan(p_forward);
    fftw_destroy_plan(p_backward);

    fftw_free(mask_data);
    fftw_free(spectrum);
    fftw_free(field);

}

void simulate_2d_abbe(const SimulationConfig& c, double *mask, std::vector<double>& total_intensity) {
    int size = c.Nx * c.Ny;
    fftw_complex *mask_data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex *spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex *tempSpectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size); // Window for lowpass filter should be changed by sigma value, so store fft result in the temp
    fftw_complex *field = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

    // 1. Initialize Mask and compute Mask Spectrum (Forward FFT)
    // (User would fill mask_data here)
    fftw_plan p_forward = fftw_plan_dft_2d(c.Nx, c.Ny, mask_data, spectrum, FFTW_FORWARD, FFTW_ESTIMATE);
    for (int i = 0; i < size; i++) {
        mask_data[i][0] = mask[i];
        mask_data[i][1] = 0.0;
    }
    fftw_execute(p_forward);

    for (int i = 0; i < size; i++) {
        tempSpectrum[i][0] = spectrum[i][0];
        tempSpectrum[i][1] = spectrum[i][1];
    }

    // 3. Loop over 2D Source Grid (Abbe sum)
    int source_points = 0;

    const double pupilRx = (c.NA / c.wavelength) * (c.Nx * c.dx);
    const double pupilRy = (c.NA / c.wavelength) * (c.Ny * c.dy);
    std::cout << "PupilRx:" << pupilRx << std::endl;

    double ds = 0.1; // Source sampling step
    for (double sx = -c.sigma; sx <= c.sigma; sx += ds) {
        for (double sy = -c.sigma; sy <= c.sigma; sy += ds) {
            double d = sx*sx + sy*sy;
            if (d > c.sigma*c.sigma || d < c.innerSigma*c.innerSigma) continue;

            for (int i = 0; i < size; i++) {
                spectrum[i][0] = tempSpectrum[i][0];
                spectrum[i][1] = tempSpectrum[i][1];
            }

            source_points++;
            // Apply Shift + Pupil + IFFT Logic:
            // a) Shift 'spectrum' by (sx, sy)
            int shiftX = std::lround(sx * (c.NA / c.wavelength) * c.Nx * c.dx);
            int shiftY = std::lround(sy * (c.NA / c.wavelength) * c.Ny * c.dy);

            // b) Multiply by Pupil(fx, fy) where f^2 + g^2 <= (NA/lambda)^2
            for (int y = 0; y < c.Ny; y++) {
                for (int x = 0; x < c.Nx; x++) {
                    int xc = c.Nx;
                    int yc = c.Ny;

                    // first origin
                    int x0 = 0 + shiftX, y0 = 0 + shiftY;
                    int x1 = std::abs(x - x0);
                    int y1 = std::abs(y - y0);
                    xc = std::min(xc, x1);
                    yc = std::min(yc, y1);

                    // 2nd origin
                    x0 = 0 + shiftX, y0 = c.Ny + shiftY;
                    x1 = std::abs(x - x0);
                    y1 = std::abs(y - y0);
                    xc = std::min(xc, x1);
                    yc = std::min(yc, y1);

                    // 3rd origin
                    x0 = c.Nx + shiftX, y0 = 0 + shiftY;
                    x1 = std::abs(x - x0);
                    y1 = std::abs(y - y0);
                    xc = std::min(xc, x1);
                    yc = std::min(yc, y1);

                    // 4th origin
                    x0 = c.Nx + shiftX, y0 = c.Ny + shiftY;
                    x1 = std::abs(x - x0);
                    y1 = std::abs(y - y0);
                    xc = std::min(xc, x1);
                    yc = std::min(yc, y1);

                    double r2 = std::sqrt((double)(xc * xc) / (pupilRx * pupilRx) + (double)(yc * yc) / (pupilRy * pupilRy));

                    if (r2 > 1.0) {
                         spectrum[y * c.Nx + x][0] = 0;
                         spectrum[y * c.Nx + x][1] = 0;
                    }
                }
            }

            // c) fftw_execute(p_backward);
            fftw_plan p_backward = fftw_plan_dft_2d(c.Nx, c.Ny, spectrum, field, FFTW_BACKWARD, FFTW_ESTIMATE);

            fftw_execute(p_backward);

            for (int i = 0; i < size; ++i) {
                double mag = std::sqrt(field[i][0]*field[i][0] + field[i][1]*field[i][1]);
                total_intensity[i] += mag;
            }

            fftw_destroy_plan(p_backward);
        }
    }

    for (int i = 0; i < size; ++i) {
        total_intensity[i] /= (double)source_points;
    }

    // Cleanup
    fftw_destroy_plan(p_forward);

    fftw_free(mask_data);
    fftw_free(spectrum);
    fftw_free(tempSpectrum);
    fftw_free(field);
}
