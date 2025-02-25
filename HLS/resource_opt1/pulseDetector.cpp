#include "pulseDetector.hpp"
#include <cmath>
#include <complex>

void pulseDetector(complex_stream& RxSignal, complex_stream& CorrFilter, fixed_point& peak, int& location) {
#pragma HLS DATAFLOW

    complex_fixed_point dataBuff[FILTER_LENGTH];
#pragma HLS ARRAY_PARTITION variable=dataBuff complete dim=1

    for (int i = 0; i < FILTER_LENGTH; i++) {
        dataBuff[i] = 0;
    }

    complex_fixed_point corrFilterBuff[FILTER_LENGTH];
#pragma HLS ARRAY_PARTITION variable=corrFilterBuff complete dim=1
    for (int i = 0; i < FILTER_LENGTH; i++) {
        corrFilterBuff[i] = CorrFilter.read();
    }

    fixed_point current_peak = 0;
    int current_location = 0;

    for (int n = 0; n < SIGNAL_LENGTH; n++) {
#pragma HLS PIPELINE II=1
        complex_fixed_point rx_sample = RxSignal.read();

        // Shift the buffer
        for (int j = FILTER_LENGTH - 1; j > 0; j--) {
            dataBuff[j] = dataBuff[j - 1];
        }
        dataBuff[0] = rx_sample;

        complex_fixed_point sum = {0, 0};
        // Perform the filtering
        for (int j = 0; j < FILTER_LENGTH; j++) {
            sum += dataBuff[j] * corrFilterBuff[j];
        }
        fixed_point sum_mag_squared = sum.real() * sum.real() + sum.imag() * sum.imag();

        // peak finding logic
        if (sum_mag_squared > current_peak) {
            current_peak = sum_mag_squared;
            current_location = n;
        }
    }

    peak = current_peak;
    location = current_location;
}
