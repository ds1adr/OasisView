#include <vector>
#include <complex>
#include <fftw3.h>
#include <cmath>
#include "Simulator.h"

void shift_fft(fftw_complex* shifted, fftw_complex* origin, const SimulationConfig& c, int shiftX, int shiftY) {
    for (int i = 0; i < c.N; i++) {
        int ii = (i + c.N + shiftX) % c.N;
        for (int j = 0; j < c.N; j++) {
            int jj = (i + c.N + shiftY) % c.N;
            shifted[ii * c.N + jj][0] = origin[i * c.N + j][0];
            shifted[ii * c.N + jj][1] = origin[i * c.N + j][1];
        }
    }
}

/*
 * std::vector<double> total_intensity(size, 0.0);
 */
void simulate_2d_abbe(const SimulationConfig& c, fftw_complex *mask_data, std::vector<double>& total_intensity) {
    int size = c.N * c.N;
    // fftw_complex *mask_data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex *spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex *field = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

    // 1. Initialize Mask and compute Mask Spectrum (Forward FFT)
    // (User would fill mask_data here)
    fftw_plan p_forward = fftw_plan_dft_2d(c.N, c.N, mask_data, spectrum, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p_forward);


    // 3. Loop over 2D Source Grid (Abbe sum)
    int source_points = 0;

    const double pupilRx = (c.NA / c.wavelength) * (c.N * c.dx);
    const double pupilRy = (c.NA / c.wavelength) * (c.N * c.dy);

    double ds = 0.1; // Source sampling step
    for (double sx = -c.sigma; sx <= c.sigma; sx += ds) {
        for (double sy = -c.sigma; sy <= c.sigma; sy += ds) {
            if (sx*sx + sy*sy > c.sigma*c.sigma) continue; // Circular source check

            source_points++;
            // Apply Shift + Pupil + IFFT Logic:
            // a) Shift 'spectrum' by (sx, sy)
            int shiftX = std::lround(sx * (c.NA / c.wavelength) * c.N * c.dx);
            int shiftY = std::lround(sy * (c.NA / c.wavelength) * c.N * c.dy);

            fftw_complex *eSpectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

            // b) Multiply by Pupil(fx, fy) where f^2 + g^2 <= (NA/lambda)^2
            for (int x = 0; x < c.N; x ++) {
                for (int y = 0; y < c.N; y++) {
                    int xc = x - c.N/2;
                    int yc = y - c.N/2;

                    double r2 = (double)(xc * xc) / (pupilRx * pupilRx) + (double)(yc * yc) / (pupilRy * pupilRy);

                    if (r2 > 1.0) continue;

                    int shiftedX = (x + c.N - shiftX) % c.N;
                    int shiftedY = (y + c.N - shiftY) % c.N;

                    eSpectrum[x * c.N + y][0] = spectrum[shiftedX * c.N + shiftedY][0];
                    eSpectrum[x * c.N + y][1] = spectrum[shiftedX + c.N + shiftedY][1];

                }
            }

            // c) fftw_execute(p_backward);
            fftw_plan p_backward = fftw_plan_dft_2d(c.N, c.N, eSpectrum, field, FFTW_BACKWARD, FFTW_ESTIMATE);
            fftw_execute(p_backward);

            fftw_free(eSpectrum);
            fftw_destroy_plan(p_backward);

            for (int i = 0; i < size; ++i) {
                double mag = std::sqrt(field[i][0]*field[i][0] + field[i][1]*field[i][1]);
                total_intensity[i] += mag * mag;
            }
        }
    }

    // Cleanup
    fftw_destroy_plan(p_forward);

    fftw_free(spectrum); fftw_free(field);
}

void simulate_2d_test(const SimulationConfig& c, fftw_complex *mask_data, std::vector<double>& total_intensity) {
    int size = c.N * c.N;
    // fftw_complex *mask_data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex *spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex *field = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

    // 1. Initialize Mask and compute Mask Spectrum (Forward FFT)
    // (User would fill mask_data here)
    fftw_plan p_forward = fftw_plan_dft_2d(c.N, c.N, mask_data, spectrum, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p_forward);


    // 3. Loop over 2D Source Grid (Abbe sum)
    int source_points = 0;

    const double pupilRx = (c.NA / c.wavelength) * (c.N * c.dx);
    const double pupilRy = (c.NA / c.wavelength) * (c.N * c.dy);

    double ds = 0.1; // Source sampling step
    // for (double sx = -c.sigma; sx <= c.sigma; sx += ds) {
    //     for (double sy = -c.sigma; sy <= c.sigma; sy += ds) {
            double sx = 0, sy = 0;

            source_points++;
            // Apply Shift + Pupil + IFFT Logic:
            // a) Shift 'spectrum' by (sx, sy)
            int shiftX = std::lround(sx * (c.NA / c.wavelength) * c.N * c.dx);
            int shiftY = std::lround(sy * (c.NA / c.wavelength) * c.N * c.dy);

            fftw_complex *eSpectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

            // b) Multiply by Pupil(fx, fy) where f^2 + g^2 <= (NA/lambda)^2
            for (int x = 0; x < c.N; x ++) {
                for (int y = 0; y < c.N; y++) {
                    int xc = x - c.N/2;
                    int yc = y - c.N/2;

                    double r2 = (double)(xc * xc) / (pupilRx * pupilRx) + (double)(yc * yc) / (pupilRy * pupilRy);

                    if (r2 > 1.0) continue;

                    int shiftedX = (x + c.N - shiftX) % c.N;
                    int shiftedY = (y + c.N - shiftY) % c.N;

                    eSpectrum[x * c.N + y][0] = spectrum[shiftedX * c.N + shiftedY][0];
                    eSpectrum[x * c.N + y][1] = spectrum[shiftedX + c.N + shiftedY][1];

                }
            }

            // c) fftw_execute(p_backward);
            fftw_plan p_backward = fftw_plan_dft_2d(c.N, c.N, eSpectrum, field, FFTW_BACKWARD, FFTW_ESTIMATE);
            fftw_execute(p_backward);

            fftw_free(eSpectrum);
            fftw_destroy_plan(p_backward);

            for (int i = 0; i < size; ++i) {
                double mag = std::sqrt(field[i][0]*field[i][0] + field[i][1]*field[i][1]);
                total_intensity[i] += mag * mag;
            }
    //     }
    // }

    // Cleanup
    fftw_destroy_plan(p_forward);

    fftw_free(spectrum); fftw_free(field);
}