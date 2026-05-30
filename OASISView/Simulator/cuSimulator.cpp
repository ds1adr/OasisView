#include <cstring>
#include <iostream>

#if defined(__CUDA_ARCH__)

#include "cuSimulator.h"

void cu_simulate_2d_abbe(const SimulationConfig& c, double *mask, std::vector<double>& total_intensity) {
    int size = c.Nx * c.Ny;
    cufftHandle p_forward;
    cufftHandle p_backward;
    cufftComplex* mask_data;
    cufftComplex* spectrum;
    cufftComplex* tempSpectrum;
    cufftComplex* field;

    cudaMalloc((void**)& mask_data, sizeof(cufftComplex) * size);
    cudaMalloc((void**)& spectrum, sizeof(cufftComplex) * size);
    cudaMalloc((void**)& tempSpectrum, sizeof(cufftComplex) * size);
    cudaMalloc((void**)& field, sizeof(cufftComplex) * size);

    // 1. Initialize Mask and compute Mask Spectrum (Forward FFT)
    // (User would fill mask_data here)
    cufftPlan2d(&p_forward, c.Nx, c.Ny, CUFFT_C2C);

    for (int i = 0; i < size; i++) {
        mask_data[i].x = mask[i];
        mask_data[i].y = 0.0;
    }
    cufftExecC2C(p_forward, mask_data, spectrum, CUFFT_FORWARD);

    for (int i = 0; i < size; i++) {
        tempSpectrum[i].x = spectrum[i].x;
        tempSpectrum[i].y = spectrum[i].y;
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
                spectrum[i].x = tempSpectrum[i].x;
                spectrum[i].y = tempSpectrum[i].y;
            }

            source_points++;
            // Apply Shift + Pupil + IFFT Logic:
            // a) Shift 'spectrum' by (sx, sy)
            int shiftX = std::lround(sx * (c.NA / c.wavelength) * c.Nx * c.dx);
            int shiftY = std::lround(sy * (c.NA / c.wavelength) * c.Ny * c.dy);

            std::cout << "ShiftX:" << shiftX << " ShiftY:" << shiftY << std::endl;

            fftw_complex *eSpectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
            std::memset((void*)eSpectrum, 0, sizeof(fftw_complex)*size);

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
                         spectrum[y * c.Nx + x].x = 0;
                         spectrum[y * c.Nx + x].y = 0;
                    }
                }
            }

            // c) fftw_execute(p_backward);
            cufftPlan2d(&p_backward, c.Nx, c.Ny, CUFFT_C2C);

            cufftExecC2C(p_backward, spectrum, field, CUFFT_INVERSE);

            for (int i = 0; i < size; ++i) {
                double mag = std::sqrt(field[i].x*field[i].x + field[i].y*field[i].y);
                total_intensity[i] += mag;
            }

            fftw_free(eSpectrum);
            cufftDestroy(p_backward);
        }
    }

    for (int i = 0; i < size; ++i) {
        total_intensity[i] /= (double)source_points;
    }

    // Cleanup
    cufftDestroy(p_forward);

    cudaFree(mask_data);
    cudaFree(spectrum);
    cudaFree(tempSpectrum);
    cudaFree(field);
}

#endif