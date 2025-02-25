#include "pulseDetector.hpp"
#include <cmath>
#include <complex>

const fixed_point corrFilterBuff[FILTER_LENGTH][3] = {
#include "corrFilterArray.txt"
};

void matchFilter(complex_stream& RxSignal, real_stream& FilterOut) {
    complex_fixed_point dataBuff[FILTER_LENGTH];
#pragma HLS ARRAY_PARTITION variable=dataBuff complete dim=1

    for (int i = 0; i < FILTER_LENGTH; i++) {
        dataBuff[i] = 0;
    }

    for (int i = 0; i < SIGNAL_LENGTH; i++) {
#pragma HLS PIPELINE II=1
        complex_fixed_point rx_sample = RxSignal.read();

        // Shift the buffer
        for (int j = FILTER_LENGTH - 1; j > 0; j--) {
            dataBuff[j] = dataBuff[j - 1];
        }
        dataBuff[0] = rx_sample;

        fixed_point conv_real = 0;
        fixed_point conv_imag = 0;
        fixed_point conv_plus = 0;

        // Perform the filtering
        for (int j = 0; j < FILTER_LENGTH; j++) {
            conv_real += dataBuff[j].real() * corrFilterBuff[j][0];
            conv_imag += dataBuff[j].imag() * corrFilterBuff[j][1];
            conv_plus += (dataBuff[j].real() + dataBuff[j].imag()) * corrFilterBuff[j][2];
        }

        complex_fixed_point convSum;
        convSum.real(conv_real - conv_plus);
        convSum.imag(conv_imag + conv_plus);

        fixed_point sum_mag_squared = convSum.real() * convSum.real() + convSum.imag() * convSum.imag();
        FilterOut.write(sum_mag_squared); // Magnitude squared
    }
}

void peakFinder(real_stream& FilterOut, fixed_point& peak, int& location) {
    fixed_point current_peak = 0;
    int current_location = 0;

    for (int n = 0; n < SIGNAL_LENGTH; n++) {
#pragma HLS PIPELINE II=1
        fixed_point magVal = FilterOut.read();

        if (magVal > current_peak) {
            current_peak = magVal;
            current_location = n;
        }
    }

    peak = current_peak;
    location = current_location;
}

void pulseDetector(complex_stream& RxSignal, fixed_point& peak, int& location) {
#pragma HLS DATAFLOW
    real_stream FilterOut;
#pragma HLS STREAM variable=FilterOut depth=4 dim=1

    matchFilter(RxSignal, FilterOut);
    peakFinder(FilterOut, peak, location);
}
