#include <vector>
#include <complex>
#include <fftw3.h>
#include <cmath>

struct Config {
    double wavelength = 193.0;
    double NA = 1.35;
    double sigma = 0.7; // Partial coherence
    int N = 512;        // Simulation grid size (NxN)
};

void simulate_2d_abbe(const Config& c) {
    int size = c.N * c.N;
    fftw_complex *mask_data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex *spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex *field = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    std::vector<double> total_intensity(size, 0.0);

    // 1. Initialize Mask and compute Mask Spectrum (Forward FFT)
    // (User would fill mask_data here)
    fftw_plan p_forward = fftw_plan_dft_2d(c.N, c.N, mask_data, spectrum, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p_forward);

    // 2. Setup Inverse Plan
    fftw_plan p_backward = fftw_plan_dft_2d(c.N, c.N, spectrum, field, FFTW_BACKWARD, FFTW_ESTIMATE);

    // 3. Loop over 2D Source Grid (Abbe sum)
    int source_points = 0;
    double ds = 0.1; // Source sampling step
    for (double sx = -c.sigma; sx <= c.sigma; sx += ds) {
        for (double sy = -c.sigma; sy <= c.sigma; sy += ds) {
            if (sx*sx + sy*sy > c.sigma*c.sigma) continue; // Circular source check

            source_points++;
            // Apply Shift + Pupil + IFFT Logic:
            // In a full implementation, you would:
            // a) Shift 'spectrum' by (sx, sy)
            // b) Multiply by Pupil(fx, fy) where f^2 + g^2 <= (NA/lambda)^2
            // c) fftw_execute(p_backward);

            for (int i = 0; i < size; ++i) {
                double mag = std::sqrt(field[i][0]*field[i][0] + field[i][1]*field[i][1]);
                total_intensity[i] += mag * mag;
            }
        }
    }

    // Cleanup
    fftw_destroy_plan(p_forward);
    fftw_destroy_plan(p_backward);
    fftw_free(mask_data); fftw_free(spectrum); fftw_free(field);
}
