#ifndef PULSE_DETECTOR_HPP
#define PULSE_DETECTOR_HPP

#include <ap_fixed.h>
#include <hls_stream.h>

// Define fixed-point data types
typedef ap_fixed<18, 2> fixed_point; // Example: 16-bit fixed-point with 8 integer bits
typedef std::complex<fixed_point> complex_fixed_point;

// Define float data types
typedef float float_point;
typedef std::complex<float_point> complex_float_point;

// Define stream types
typedef hls::stream<complex_fixed_point> complex_stream;
typedef hls::stream<fixed_point> real_stream;
typedef hls::stream<int> int_stream;

// Define parameters (example) using macro definitions
#define FILTER_LENGTH 64
#define SIGNAL_LENGTH 5000

// Define constant array
//const fixed_point corrFilterBuff[FILTER_LENGTH][3] = { /* Initialize with appropriate values */ };

// Function declarations
void matchFilter(complex_stream& RxSignal, real_stream& FilterOut);
void peakFinder(real_stream& FilterOut, fixed_point& peak, int& location);
void pulseDetector(complex_stream& RxSignal, fixed_point& peak, int& location);

#endif
