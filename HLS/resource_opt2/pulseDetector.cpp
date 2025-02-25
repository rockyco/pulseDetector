#include "pulseDetector.hpp"
#include <cmath>
#include <complex>

complex_fixed_point corrFilterBuff[FILTER_LENGTH] = {
    complex_fixed_point(-0.00494385, 0.0148315),
    complex_fixed_point(-0.015625, 0.000579834),
    complex_fixed_point(-0.000518799, -0.015625),
    complex_fixed_point(0.00012207, 0.0155945),
    complex_fixed_point(0.00253296, -0.0154419),
    complex_fixed_point(-0.0134583, 0.00796509),
    complex_fixed_point(-0.00842285, -0.0131836),
    complex_fixed_point(0.0151062, 0.00390625),
    complex_fixed_point(-0.015625, -0.000183105),
    complex_fixed_point(0.0114441, -0.0106506),
    complex_fixed_point(0.00814819, -0.0133362),
    complex_fixed_point(-0.00878906, 0.0129089),
    complex_fixed_point(-0.00668335, 0.0140991),
    complex_fixed_point(-0.00256348, -0.0154419),
    complex_fixed_point(0.000457764, 0.0155945),
    complex_fixed_point(-0.00396729, 0.0151062),
    complex_fixed_point(-0.00949097, 0.0124207),
    complex_fixed_point(-0.01474, -0.00524902),
    complex_fixed_point(-0.0155945, -0.00100708),
    complex_fixed_point(0.00601196, -0.0144348),
    complex_fixed_point(0.00436401, 0.0149841),
    complex_fixed_point(0.00149536, 0.0155334),
    complex_fixed_point(-0.0115051, -0.0105896),
    complex_fixed_point(-0.0144958, -0.00588989),
    complex_fixed_point(0.0152588, -0.00335693),
    complex_fixed_point(0.0148621, 0.00479126),
    complex_fixed_point(-0.00640869, -0.0142822),
    complex_fixed_point(-0.00531006, 0.014679),
    complex_fixed_point(0.00695801, 0.0139771),
    complex_fixed_point(0.0127869, -0.00897217),
    complex_fixed_point(0.0149536, -0.00448608),
    complex_fixed_point(-0.00265503, -0.0154114),
    complex_fixed_point(0.0152893, -0.00311279),
    complex_fixed_point(-0.00427246, 0.0150146),
    complex_fixed_point(0.00741577, -0.0137634),
    complex_fixed_point(-0.00875854, 0.0129395),
    complex_fixed_point(-0.012207, -0.00979614),
    complex_fixed_point(-0.000701904, 0.0155945),
    complex_fixed_point(0.000732422, 0.0155945),
    complex_fixed_point(-0.0067749, 0.0140686),
    complex_fixed_point(0.0142822, 0.00628662),
    complex_fixed_point(0.00909424, 0.0126648),
    complex_fixed_point(0.0152283, -0.003479),
    complex_fixed_point(-0.00872803, 0.0129395),
    complex_fixed_point(0.0151062, 0.00390625),
    complex_fixed_point(0.00408936, 0.0150757),
    complex_fixed_point(0.0134583, 0.00787354),
    complex_fixed_point(-0.0137939, -0.00738525),
    complex_fixed_point(0.00979614, -0.0121765),
    complex_fixed_point(0.00485229, 0.0148315),
    complex_fixed_point(-0.015564, -0.00143433),
    complex_fixed_point(0.0150452, 0.00415039),
    complex_fixed_point(0.0153503, 0.00286865),
    complex_fixed_point(0.00854492, -0.013092),
    complex_fixed_point(0.0152283, 0.00341797),
    complex_fixed_point(0.0150452, 0.00411987),
    complex_fixed_point(-0.0149536, 0.0045166),
    complex_fixed_point(-0.00280762, -0.0153809),
    complex_fixed_point(0.0127563, -0.00900269),
    complex_fixed_point(-0.0105286, 0.0115356),
    complex_fixed_point(0.0133667, 0.00808716),
    complex_fixed_point(0.0108948, -0.0112),
    complex_fixed_point(0.0129395, 0.00869751),
    complex_fixed_point(0.00616455, 0.0143433)
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

        complex_fixed_point sum = {0, 0};
        // Perform the filtering
        for (int j = 0; j < FILTER_LENGTH; j++) {
            // sum += dataBuff[j] * conj(corrFilterBuff[j]);
            sum += dataBuff[j] * corrFilterBuff[j];
        }
        fixed_point sum_mag_squared = sum.real() * sum.real() + sum.imag() * sum.imag();
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
